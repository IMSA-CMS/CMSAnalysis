

#include "CIAnalysis/CIStudies/interface/CandidateImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

CandidateImplementation::CandidateImplementation(const reco::Candidate* iparticle):
particle(iparticle)
{
  //std::cout << "Got to CI\n";
}

int CandidateImplementation::charge() const 
{
  // std::cout << "charge\n";
  checkIsNull();
  return particle->charge();
}


double CandidateImplementation::et() const
{
  // std::cout << "et\n";
  checkIsNull();
  return particle->et();
}

// double CandidateImplementation::eta() const
// {
//   checkIsNull();
//   return particle->eta();
// }

double CandidateImplementation::energy() const
{
  // std::cout << "energy\n";
  checkIsNull();
  return particle->energy();
}

reco::Candidate::LorentzVector CandidateImplementation::getFourVector() const
{
  // std::cout << "get four vec\n";
  checkIsNull();
  return particle->p4();
}

bool CandidateImplementation::operator==(const ParticleImplementation& userParticle) const 
{
    try
    {
      auto candidateImp = dynamic_cast<const CandidateImplementation&>(userParticle);
      return candidateImp.particle == particle;
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}
   
// }
// Particle& CandidateImplementation::operator = (const CandidateImplementation& particle2)
// {
//   particle = particle2.particle;
//   return *this;
// }

int CandidateImplementation::pdgId() const
{
  // std::cout << "pdgId\n";
  checkIsNull();
  return particle->pdgId();
}

int CandidateImplementation::status() const
{
  // std::cout << "status\n";
  checkIsNull();
  return particle->status();
}

Particle CandidateImplementation::mother() const
{
  // std::cout << "mother\n";
  checkIsNull();
  //mother of particle is often not electron/muon
  return Particle(particle->mother());
}




Particle CandidateImplementation::daughter(int i) const
{
  // std::cout << "daugters\n";
  checkIsNull();
  auto daughter = particle->daughter(i);

/*

  Particle::Type type;
  switch (daughter->pdgId()) {
  case 11:
    type = Particle::Type::Electron;
    break;
  case 13:
    type = Particle::Type::Muon;
    break;
  default:
    type = Particle::Type::None;
    break;
  }

*/

  return Particle(daughter);
}

int CandidateImplementation::numberOfDaughters() const {
  // std::cout << "Num Daugters\n";
  checkIsNull();
  return particle->numberOfDaughters();
}


Particle::Type CandidateImplementation::getType() const
{

  if (!particle)
  {
    return Particle::Type::None;
  }

  if (dynamic_cast<const pat::Muon*>(particle))
  {
    return Particle::Type::Muon;
  }

  else if (dynamic_cast<const pat::Electron*>(particle))
  {
    return Particle::Type::Electron;
  }
  else if (dynamic_cast<const pat::Photon*>(particle))
  {
    return Particle::Type::Photon;
  }

  else if (auto genp = dynamic_cast<const reco::GenParticle*>(particle))
  {
    double particleId = genp->pdgId();

    if (particleId == 11 || particleId == -11)
    {
      return Particle::Type::Electron;
    }

    else if (particleId == 13 || particleId == -13)
    {
      return Particle::Type::Muon;
    }
    else if (particleId == 22)
    {
      return Particle::Type::Photon;
    }

    else
    {
      return Particle::Type::None;
    }
  }

  else
  {
    return Particle::Type::None;
  }

}


// bool CandidateImplementation::isIsolated() const
// {
//   checkIsNull();
//   auto muon = dynamic_cast<const reco::Muon*>(particle);
//   if (!muon)
//   {
//     return false;
//   } 
//   auto isolation = muon->isolationR03();
//   if (isolation.sumPt < 0.1*muon->pt())
//   {
//     return true;
//   }
//   return false;
// }

void CandidateImplementation::checkIsNull() const
{
  if(!particle)
  {
    throw std::runtime_error("attempted to use null pointer in Particle (CI)");	
  }
}

bool CandidateImplementation::isFinalState() const 
{
    checkIsNull();
    if(auto gen = dynamic_cast<const reco::GenParticle*>(particle))
    {
      return gen->isPromptFinalState();
    }
    return true;
}