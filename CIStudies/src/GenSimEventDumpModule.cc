#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"

GenSimEventDumpModule::GenSimEventDumpModule(int inumOfEvents):
  numOfEvents(inumOfEvents)
{
  counter = 0;
}

bool GenSimEventDumpModule::process(const edm::EventBase& event)
{
  if(counter < numOfEvents || numOfEvents == -1)
  {
    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);

    printGenParticleCollection(*genParticlesHandle);
    counter++;
    return true;
  }
  else
  {
    return true;
  }
}

void GenSimEventDumpModule::writeAll()
{
}

void GenSimEventDumpModule::printGenParticleCollection(const reco::GenParticleCollection& genParts) const
{
  const reco::Candidate* daughter1 = nullptr;
  const reco::Candidate* daughter2 = nullptr;

  const reco::Candidate* mother1 = nullptr;
  const reco::Candidate* mother2 = nullptr;

  int eventIndex = 0;

  //Format
  std::cerr << "--------------------------------------------------------" << std::endl;
  std::cerr << "EVENT #" << (counter + 1) << std::endl;
  std::cerr << "--------------------------------------------------------" << std::endl;
  std::cerr << " " << std::endl;

  std::cout << std::left << std::setw(10) << "index, " << std::setw(10) << "pdfId, "
   << std::setw(10) << "status; " << std::setw(10) << "mother1; "
   << std::setw(10) << "mother2; " << std::setw(10) << "daughter1; "
   << std::setw(10) << "daughter2, " << std::setw(10) << "px, "
   << std::setw(10) << "py, " << std::setw(10) << "pz, "
   << std::setw(10) << "E, " << std::setw(5) << "mass\n";

  //Prints out all of the particles
  for(auto &part : genParts){

    //Distinguishing Mothers
    if(part.numberOfMothers() == 2)
    {
      mother1 = part.mother(0);
      mother2 = part.mother(1);
    }
    else if(part.numberOfMothers() == 1)
    {
      mother1 = part.mother(0);
      mother2 = part.mother(0);
    }
   
    //Distinguishing Daughters (There can be more than 2 daughters, max of three)
    if(part.numberOfDaughters() == 2)
    {
      daughter1 = part.daughter(0);
      daughter2 = part.daughter(1);
    }
    else if(part.numberOfDaughters() == 1)
    {
      daughter1 = part.daughter(0);
      daughter2 = part.daughter(0);
    }

    std::cout << std::setw(10) << eventIndex << ", " << std::setw(10) << part.pdgId() << ", " << std::setw(10) << part.status() << "; ";

    if(part.numberOfMothers() != 0)
    {
      std::cout << std::setw(10) << getIndexOf(mother1, genParts) << "; " << std::setw(10) << getIndexOf(mother2, genParts) << "; ";
    }
    else
    {
      std::cout << std::setw(10) << 00 << "; " << std::setw(10) << 00 << "; ";
    }

    if(part.numberOfDaughters() != 0 && part.numberOfDaughters() != 3)
    {
      std::cout << std::setw(10) << getIndexOf(daughter1, genParts) << "; " << std::setw(10) << getIndexOf(daughter2, genParts) << "; ";
    }
    else
    {
      std::cout << std::setw(10) << 00 << "; " << std::setw(10) << 00 << "; ";
    }

    std::cout << std::setw(10) << part.px() << ", " << std::setw(10) << part.py() << ", " << std::setw(10) << part.pz() << ", " << std::setw(10) << part.energy() << ", " << std::setw(10) << part.mass() << "\n";
   
    eventIndex++;
  }
}

int GenSimEventDumpModule::getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts) const
{
  int indexOf = 0;
  for(auto &possiblePart : genParts)
  {
    if(&possiblePart == part)
      return indexOf;
    indexOf++;
  }
  return -1;
}

