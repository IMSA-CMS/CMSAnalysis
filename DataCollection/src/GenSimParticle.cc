#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/CandidateImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/SimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"

GenSimParticle::GenSimParticle(const Particle& particle):
Particle(particle)
{

}

int GenSimParticle::pdgId() const
{
  checkIsNull();
  return getParticle()->pdgId();
}



int GenSimParticle::status() const
{
  checkIsNull();
  return getParticle()->status();
}

GenSimParticle GenSimParticle::mother() const
{
  checkIsNull();
  //mother of particle is often not electron/muon
  return GenSimParticle(getParticle()->mother());
}

int GenSimParticle::numberOfDaughters() const {
  checkIsNull();
  return getParticle()->numberOfDaughters();
}

GenSimParticle GenSimParticle::daughter(int i) const
{
  checkIsNull();
  auto daughter = getParticle()->daughter(i);
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

  return GenSimParticle(daughter);
}


bool GenSimParticle::isFinalState() const
{
  return getParticle()->isFinalState();
}

GenSimParticle GenSimParticle::uniqueMother() const
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

GenSimParticle GenSimParticle::finalDaughter() const
{
  GenSimParticle current = GenSimParticle(*this);
  while (current.status() != 1) {
    int di = -1;
    int dpt = 0;
    for (int i = 0; i < current.numberOfDaughters(); ++i) {
      GenSimParticle daughter = current.daughter(i);
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

GenSimParticle GenSimParticle::findMother(int motherPDGID) const
{
  bool foundMother = false;

  GenSimParticle finalMother = Particle(nullptr);  // Null Particle

  auto currentMother = mother();

  while (!foundMother)
  {
    if (!currentMother.isNotNull())  // Return a null particle if we reach an initial particle
    {
      return GenSimParticle(nullptr);
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

GenSimParticle GenSimParticle::sharedMother(int motherPDGID, GenSimParticle particle1, GenSimParticle particle2)
{
  auto mother1 = particle1.findMother(motherPDGID);  // Define these here for convenience
  auto mother2 = particle2.findMother(motherPDGID);

  if ((mother1 == mother2) && (mother1.isNotNull()) && (mother2.isNotNull()))
  {
    return mother1;
  }
  else
  {
    return GenSimParticle(nullptr);
  }
}

GenSimParticle GenSimParticle::sharedMother(int motherPDGID, std::vector<Particle> particles)
{
  if (particles.size() < 2)
  {
    return GenSimParticle(nullptr);
  }

  GenSimParticle finalMother = sharedMother(motherPDGID, particles[0], particles[1]);  // Shared mother between the first 2 particles

  // Find the shared mother between all possible particle pairs.
  // If they are all the same, then that is the shared mother.
  // If they are different, then return a null particle, since there is no particle.
  for (int i = 0; i < static_cast<int>(particles.size()); i++)
  {
    for (int j = i + 1; j < static_cast<int>(particles.size()); j++)
    {
      if (sharedMother(motherPDGID, particles[i], particles[j]) != finalMother)
      {
        return GenSimParticle(nullptr);
      }
    }
  }
  return finalMother;
}



