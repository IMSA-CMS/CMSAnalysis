#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/MuonReco/interface/Muon.h"

Particle::Particle(const reco::Candidate *iparticle, LeptonType iLeptonType)
    : particle(iparticle), leptonType(iLeptonType) {}

int Particle::charge() const {
  checkIsNull();
  return particle->charge();
}

double Particle::pt() const {
  checkIsNull();
  return particle->pt();
}

double Particle::eta() const {
  checkIsNull();
  return particle->eta();
}

double Particle::phi() const {
  checkIsNull();
  return particle->phi();
}

double Particle::et() const {
  checkIsNull();
  return particle->et();
}

double Particle::energy() const {
  checkIsNull();
  return particle->energy();
}

reco::Candidate::LorentzVector Particle::fourVector() const {
  checkIsNull();
  return particle->p4();
}

int Particle::pdgId() const {
  checkIsNull();
  return particle->pdgId();
}

int Particle::status() const {
  checkIsNull();
  return particle->status();
}

Particle Particle::mother() const {
  checkIsNull();
  // mother of particle is often not electron/muon
  return Particle(particle->mother(), Particle::LeptonType::None);
}

Particle Particle::uniqueMother() const {
  checkIsNull();
  // The mother that is not itself
  auto mom = mother();
  mom.checkIsNull();

  if (mom.pdgId() == pdgId()) {
    return mom
        .uniqueMother(); // Recursive back to itself, so it will keep going
                         // until it returns a mother that is not the particle
  }

  return mom;
}

Particle Particle::daughter(int i) const {
  checkIsNull();
  auto daughter = particle->daughter(i);
  Particle::LeptonType type;
  switch (daughter->pdgId()) {
  case 11:
    type = Particle::LeptonType::Electron;
    break;
  case 13:
    type = Particle::LeptonType::Muon;
    break;
  default:
    type = Particle::LeptonType::None;
    break;
  }

  return Particle(daughter, type);
}

int Particle::numberOfDaughters() const {
  checkIsNull();
  return particle->numberOfDaughters();
}

Particle Particle::finalDaughter() {
  Particle current = Particle(particle, Particle::LeptonType::None);
  while (current.status() != 1) {
    int di = -1;
    int dpt = 0;
    for (int i = 0; i < current.numberOfDaughters(); ++i) {
      Particle daughter = current.daughter(i);
      if (daughter.pdgId() == current.pdgId() && daughter.pt() > dpt) {
        di = i;
        dpt = daughter.pt();
      }
    }
    if (di == -1) {
      std::cout << "OH NO!!!!!!!\n";
      std::cout << current.pdgId() << '\n';
      break;
    }
    current = current.daughter(di);
  }
  return current;
}

Particle Particle::findMother(int motherPDGID)
{
  bool foundMother = false;

  Particle finalMother = Particle(nullptr, Particle::LeptonType::None);  // Null Particle

  auto currentMother = mother();

  while (!foundMother)
  {
    if (!currentMother.isNotNull())  // Return a null particle if we reach an initial particle
    {
      return Particle(nullptr, Particle::LeptonType::None);
    }
    else if (currentMother.pdgId() == motherPDGID)
    {
      finalMother = currentMother;
      foundMother = true;
    }
    else
    {
      currentMother = currentMother.mother();
    }
  }

  return finalMother;
}

Particle Particle::sharedMother(int motherPDGID, Particle particle1, Particle particle2)
{
  auto mother1 = particle1.findMother(motherPDGID);  // Define these here for convenience
  auto mother2 = particle2.findMother(motherPDGID);

  if ((mother1 == mother2) && (mother1.isNotNull()) && (mother2.isNotNull()))
  {
    return mother1;
  }
  else
  {
    return Particle(nullptr, Particle::LeptonType::None);
  }
}

Particle Particle::sharedMother(int motherPDGID, std::vector<Particle> particles)
{
  if (particles.size() < 2)
  {
    return Particle(nullptr, Particle::LeptonType::None);
  }

  Particle finalMother = sharedMother(motherPDGID, particles[0], particles[1]);  // Shared mother between the first 2 particles

  // Find the shared mother between all possible particle pairs.
  // If they are all the same, then that is the shared mother.
  // If they are different, then return a null particle, since there is no particle.
  for (int i = 0; i < static_cast<int>(particles.size()); i++)
  {
    for (int j = i + 1; j < static_cast<int>(particles.size()); j++)
    {
      if (sharedMother(motherPDGID, particles[i], particles[j]) != finalMother)
      {
        return Particle(nullptr, Particle::LeptonType::None);
      }
    }
  }
  return finalMother;
}

Particle::LeptonType Particle::getLeptonType() const
{
  checkIsNull();
  return leptonType;
}

Particle::BarrelState Particle::getBarrelState() const {
  checkIsNull();
  double etaValue = std::abs(eta());
  if (leptonType == LeptonType::Muon) {
    if (etaValue < 1.2) {
      return Particle::BarrelState::Barrel;
    } else {
      return Particle::BarrelState::Endcap;
    }
  } else if (leptonType == LeptonType::Electron) {
    if (etaValue < 1.4442) {
      return Particle::BarrelState::Barrel;
    } else if (etaValue > 1.562) {
      return Particle::BarrelState::Endcap;
    }
  }
  return Particle::BarrelState::None;
}

bool Particle::isIsolated() const
{
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  if (!muon)
  {
    return false;
  } 
  auto isolation = muon->isolationR03();
  if (isolation.sumPt < 0.1*muon->pt())
  {
    return true;
  }
  return false;
}

void Particle::checkIsNull() const
{
  if(!particle)
  {
    throw std::runtime_error("attempted to use null pointer in Particle");	
  }
}
