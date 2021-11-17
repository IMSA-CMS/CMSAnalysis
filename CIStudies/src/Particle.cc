#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CIAnalysis/CIStudies/interface/ParticleImplementation.hh"
#include "CIAnalysis/CIStudies/interface/CandidateImplementation.hh"
#include "CIAnalysis/CIStudies/interface/SimpleImplementation.hh"

Particle::Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type): 
particle(std::make_shared<SimpleImplementation>(vec, charge, type))
{
  
}

Particle::Particle(const reco::Candidate* iparticle):
particle(std::make_shared<CandidateImplementation>(iparticle))
{
  //std::cout << "Got to Particle\n" << particle << "\n";
}

Particle::Particle(const Particle& particle1):
particle(particle1.particle){}

Particle& Particle::operator = (const Particle& particle2)
{
  particle = particle2.particle;
  return *this;
}

bool Particle::operator == (const Particle& p1) const
{
  return *particle == *(p1.particle);
}

Particle Particle::uniqueMother() const
{
  checkIsNull();
  // The mother that is not itself
  auto mom = mother();
  mom.checkIsNull();

  if (mom.pdgId() == pdgId())
  {
    return mom.uniqueMother(); //Recursive back to itself, so it will keep going until it returns a mother that is not the particle
  }

  return mom;
}


Particle Particle::finalDaughter() {
  Particle current = Particle(*this);
  while (current.status() != 1) {
    int di = -1;
    int dpt = 0;
    for (int i = 0; i < current.numberOfDaughters(); ++i) {
      Particle daughter = current.daughter(i);
      if (daughter.pdgId() == current.pdgId() && daughter.getPt() > dpt) {
        di = i;
        dpt = daughter.getPt();
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

  Particle finalMother = Particle(nullptr);  // Null Particle

  auto currentMother = mother();

  while (!foundMother)
  {
    if (!currentMother.isNotNull())  // Return a null particle if we reach an initial particle
    {
      return Particle(nullptr);
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
    return Particle(nullptr);
  }
}

Particle Particle::sharedMother(int motherPDGID, std::vector<Particle> particles)
{
  if (particles.size() < 2)
  {
    return Particle(nullptr);
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
        return Particle(nullptr);
      }
    }
  }
  return finalMother;
}

bool Particle::isNotNull() const 
{
  return particle->isNotNull();
}

void Particle::checkIsNull() const
{
  // std::cout << "This is check is null (particle)\n" << particle << "\n";
  if(!particle->isNotNull())
  {
    throw std::runtime_error("attempted to use null pointer in Particle (Particle)");	
  }
}

double Particle::getPt() const
{
  checkIsNull();
  return particle->getFourVector().Pt();
}

double Particle::getPhi() const
{
  checkIsNull();
  return particle->getFourVector().Phi();
}

double Particle::getEta() const
{
  checkIsNull();
  return particle->getFourVector().Eta();
}
double Particle::getEt() const
{
  checkIsNull();
  return particle->getFourVector().Et();
}

double Particle::energy() const
{
  checkIsNull();
  return particle->energy();
}
double Particle::getMass() const
{
  checkIsNull();
  return particle->getFourVector().mass();
}

Particle::BarrelState Particle::getBarrelState() const
{
  checkIsNull();
  double etaValue = std::abs(getEta());
  if (getType() == Particle::Type::Muon)
  {
    if (etaValue < 1.2)
    {
      return Particle::BarrelState::Barrel;
    }
    else
    {
      return Particle::BarrelState::Endcap;
    }
  }
  else if (getType() == Particle::Type::Electron)
  {
    if (etaValue < 1.4442)
    {
      return Particle::BarrelState::Barrel;
    }
    else if (etaValue > 1.562)
    {
      return Particle::BarrelState::Endcap;
    }
  }
  return Particle::BarrelState::None;
}

Particle::Type Particle::getType() const{
    checkIsNull();
    return particle->getType();
}
// bool Particle::isIsolated() const
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
int Particle::charge() const 
{
  checkIsNull();
  return particle->charge();
}

int Particle::status() const
{
  checkIsNull();
  return particle->status();
}

Particle Particle::mother() const
{
  checkIsNull();
  //mother of particle is often not electron/muon
  return Particle(particle->mother());
}

reco::Candidate::LorentzVector Particle::getFourVector() const
{
  checkIsNull();
  return particle->getFourVector();
}

int Particle::pdgId() const
{
  checkIsNull();
  return particle->pdgId();
}

int Particle::numberOfDaughters() const {
  checkIsNull();
  return particle->numberOfDaughters();
}

Particle Particle::daughter(int i) const
{
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
bool Particle::isFinalState() const
{
  return particle->isFinalState();
}

double Particle::getDeltaR(Particle part) const
{
  return reco::deltaR(part.getFourVector(), getFourVector());
}
