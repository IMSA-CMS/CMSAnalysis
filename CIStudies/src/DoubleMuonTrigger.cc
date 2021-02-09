#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

DoubleMuonTrigger::DoubleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff, double iSecondPTCutoff) :
  Trigger("Double Muon Trigger", iRecoMod),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::LeptonType::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  auto particlesVec = particles.getParticles(); // Vector of Particles

  double highestPT = particles.getLeadingTransverseMomentum();
  // At this point, I need to find the particle with this transverse momentum and remove it from particlesVec

  // Create another ParticleCollection with the updated particlesVec and use it to get the secondHighestPT in the next line  
  ParticleCollection particlesWithoutHighestPT;

  for (auto particle : particlesVec)
  {
    if (particle.pt() != highestPT)
    {
      particlesWithoutHighestPT.addParticle(particle);
    }
  }

  double secondHighestPT = particlesWithoutHighestPT.getLeadingTransverseMomentum();

  if ((highestPT >= pTCutoff) && (secondHighestPT >= secondPTCutoff))  // The particle passes the trigger
  {
    return true;
  }

  else
  {
    return false;
  }
}
