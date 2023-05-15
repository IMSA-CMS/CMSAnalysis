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
#include "CMSAnalysis/DataCollection/interface/EventDumpModule.hh"

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
  return GenSimParticle(daughter);
}

std::vector<GenSimParticle> GenSimParticle::getDaughters() const
{
  checkIsNull();
  std::vector<GenSimParticle> daughters;
  for(int i = 0; i < getParticle()->numberOfDaughters(); i++)
  {
    daughters.push_back(getParticle()->daughter(i));
  }
  return daughters;
}


bool GenSimParticle::isFinalState() const
{
  return getParticle()->isFinalState();
}
bool GenSimParticle::hasMother()
{
  return getParticle()->doesHaveMother();
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
  while (current.status() != 1) 
  {
    int di = -1;
    for (int i = 0; i < current.numberOfDaughters(); ++i)
    {
      GenSimParticle daughter = current.daughter(i);
      if (daughter.pdgId() == current.pdgId()) 
      {
        di = i;
      }
    }
    if (di == -1) 
    {
      break;
    }
    current = current.daughter(di);
  }
  return current;
}

GenSimParticle GenSimParticle::findMother(int motherPDGID) const
{
  bool foundMother = false;

  GenSimParticle finalMother = Particle::nullParticle();  // Null Particle

  auto currentMother = mother();

  while (!foundMother)
  {
    if (!currentMother.isNotNull())  // Return a null particle if we reach an initial particle
    {
      return Particle::nullParticle();
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
    return Particle::nullParticle();
  }
}

GenSimParticle GenSimParticle::sharedMother(int motherPDGID, std::vector<Particle> particles)
{
  if (particles.size() < 2)
  {
    return Particle::nullParticle();
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
        return Particle::nullParticle();
      }
    }
  }
  return finalMother;
}

std::ostream& operator<<(std::ostream& str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles)
{
  GenSimParticle part = genParticles.first;
  std::vector<GenSimParticle> genParts = genParticles.second;
  str << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";
  //print mothers
  str << std::setw(18) << "0" << "| ";
    //str << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";
    // Print daughters
    //formatDaughterParticles(part, particleGroup) was replaced by function not working string
  str << std::setw(18) << EventDumpModule::formatDaughterParticles(part, genParts) << "| ";
    // Particle properties
  str << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";
  str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();
  return str;
}

std::ostream& operator<<(std::ostream& str, const GenSimParticle part)
{
  str << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";
  //print mothers
  str << std::setw(18) << "" << "| ";
    //str << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";
    // Print daughters
    //formatDaughterParticles(part, particleGroup) was replaced by function not working string
  str << std::setw(18) << "" << "| ";
    // Particle properties
  str << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";
  str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();
  return str;
}