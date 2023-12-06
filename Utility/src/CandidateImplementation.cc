#include "CMSAnalysis/Utility/interface/CandidateImplementation.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

CandidateImplementation::CandidateImplementation(
    const reco::Candidate *iparticle)
    : particle(iparticle) {
}

int CandidateImplementation::charge() const {
  checkIsNull();
  return particle->charge();
}

// double CandidateImplementation::isolation() const {
//   if (getType() == ParticleType::muon()) {
//     auto muon = dynamic_cast<const reco::Muon *>(particle);
//     if (!muon)
//     {
//       return -1;
//     }
//     return muon->isolationR03().sumPt / particle->pt();
//   } 
//   else if (getType() == ParticleType::electron()) 
//   {
//     auto elec = dynamic_cast<const reco::GsfElectron *>(particle);
//         if (!elec)
//     {
//       return -1;
//     }
//     return elec->dr03TkSumPt() / particle->pt();
//   }
//   return -1;
// }

reco::Candidate::LorentzVector CandidateImplementation::getFourVector() const {
  checkIsNull();
  return particle->p4();
}

bool CandidateImplementation::operator==(
    const ParticleImplementation &userParticle) const {
  try {
      auto candidateImp =
        dynamic_cast<const CandidateImplementation &>(userParticle);
      return candidateImp.particle == particle;
  } catch (std::bad_cast &) {
    return false;
  }
}

int CandidateImplementation::pdgId() const {
  checkIsNull();
  return particle->pdgId();
}

int CandidateImplementation::status() const {
  checkIsNull();
  return particle->status();
}

Particle CandidateImplementation::mother() const {
  checkIsNull();
  // mother of particle is often not electron/muon

  const reco::Candidate* mother = particle->mother();
  Particle particleMother = Particle(mother);
  return particleMother;
  
}

Particle CandidateImplementation::daughter(int i) const {
  // std::cout << "daugters\n";
  checkIsNull();
  auto daughter = particle->daughter(i);
  /* if (!daughter)
  {
    throw std::runtime_error("null");
  }
*/
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

const ParticleType& CandidateImplementation::getType() const {
  if (!particle) {
    return ParticleType::none();
  }

  if (dynamic_cast<const pat::Muon*>(particle)) {
    return ParticleType::muon();
  }

  else if (dynamic_cast<const pat::Electron*>(particle)) {
    return ParticleType::electron();
  } else if (dynamic_cast<const pat::Photon*>(particle)) {
    return ParticleType::photon();
  }
  else if (auto genp = dynamic_cast<const reco::GenParticle*>(particle))
  {
    //std::cout << "CI call identify type\n";
    return ParticleType::getPDGType(genp->pdgId());
  }
  else {
    return ParticleType::none();
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

void CandidateImplementation::checkIsNull() const {
  if (!particle) {
    throw std::runtime_error("attempted to use null pointer in Particle (CI)");
  }
}

bool CandidateImplementation::isFinalState() const {
  checkIsNull();
  if (auto gen = dynamic_cast<const reco::GenParticle *>(particle)) {
    return gen->isPromptFinalState();
  }
  return true;
}

const pat::Muon* CandidateImplementation::getMuon() const {
  if(getType() == ParticleType::muon()) {
	return dynamic_cast<const pat::Muon *>(particle);
  }
  else {
	return 0;
  }
}

const pat::Electron* CandidateImplementation::getElectron() const {
  if(getType() == ParticleType::electron()) {
	return dynamic_cast<const pat::Electron *>(particle);
  }
  else {
        return 0;
  }
}

Particle::SelectionFit CandidateImplementation::getSelectionFit() const {
  const pat::Muon* muon = CandidateImplementation::getMuon();
  const pat::Electron* electron = CandidateImplementation::getElectron();
  if(muon)
  {
    if(muon->passed(reco::Muon::CutBasedIdTight)) {
      return Particle::SelectionFit::Tight;
    }
    else if	(muon->passed(reco::Muon::CutBasedIdMedium)) {
      return Particle::SelectionFit::Medium;
    }
    else if(muon->passed(reco::Muon::CutBasedIdLoose)) {
      return Particle::SelectionFit::Loose;
    }
    else {
      return Particle::SelectionFit::None;
    }
  }
  else if(electron) {
    if(electron->electronID("cutBasedElectronID-Fall17-94X-V2-tight") == 1) {
      return Particle::SelectionFit::Tight;
    }
    else if(electron->electronID("cutBasedElectronID-Fall17-94X-V2-medium") == 1) {
      return Particle::SelectionFit::Medium;
    }
    else if(electron->electronID("cutBasedElectronID-Fall17-94X-V2-loose") == 1) {
      return Particle::SelectionFit::Loose;
    }
    else {
      return Particle::SelectionFit::None;
    }
  }
  else {
	throw std::runtime_error("Particle is not electron or muon");
  }
}
