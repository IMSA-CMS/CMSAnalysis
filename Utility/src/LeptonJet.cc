#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

LeptonJet::LeptonJet():
RecoParticle(std::make_shared<LeptonJetImplementation>()){
  
}

LeptonJet::LeptonJet (const Particle& particle):
RecoParticle(particle){}



bool LeptonJet::operator==(LeptonJet userJet) const
{
  return cast() == userJet.getParticleImplementation();
}

std::shared_ptr<LeptonJetImplementation> LeptonJet::cast() const
{
  auto pointer = std::dynamic_pointer_cast<LeptonJetImplementation>(getParticle());
  if(pointer)
  {
    return pointer;
  }
  else
  {
    throw std::runtime_error("Null pointer");
  }
}

double LeptonJet::getMass() const
{
  auto leptons = getParticles();
  double leptonJetInvMass = 0;
  ParticleCollection<Particle> collection(leptons);
  leptonJetInvMass = collection.calculateAllLeptonInvariantMass();
  return leptonJetInvMass;
}


double LeptonJet::getDeltaR() const
{
    auto leptons = getParticles();
    double maxDeltaR = 0;
    for (size_t i = 0 ; i < leptons.size(); ++i)
    {
        for (size_t j = i + 1; j < leptons.size(); ++j)
        {
            double deltaR = leptons[i].getDeltaR(leptons[j]);
            if (deltaR > maxDeltaR)
                maxDeltaR = deltaR;
        }
    }

    return maxDeltaR;
}