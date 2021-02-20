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
  int eventElement = 1;

  // Format
  std::cerr << "--------------------------------------------------------" << std::endl;
  std::cerr << "EVENT #" << (counter + 1) << std::endl;
  std::cerr << "--------------------------------------------------------" << std::endl;

  std::cout << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
   << std::setw(10) << "| status" << std::setw(10) << "| mother1"
   << std::setw(10) << "| mother2" << std::setw(12) << "| daughter1"
   << std::setw(12) << "| daughter2" << std::setw(15) << "| px"
   << std::setw(15) << "| py" << std::setw(15) << "| pz"
   << std::setw(15) << "| E" << std::setw(5) << "| mass\n";

  //Prints out all of the particles
  for(auto &part : genParts){
    
    // Event Elements
    int daughter1 = 0;
    int daughter2 = 0;

    int mother1 = 0;
    int mother2 = 0;

    // Distinguishing Mothers
    if(part.numberOfMothers() != 0)
    {
      mother1 = 1 + getIndexOf(part.mother(0), genParts);
      mother2 = 1 + getLatestIndexOfMothers(part, genParts);
    }

    // Debug
    //std::cout << part.numberOfMothers() << ", " << part.numberOfDaughters() << std::endl;
   
    // Distinguishing Daughters
    if(part.numberOfDaughters() != 0)
    {
      daughter1 = 1 + getIndexOf(part.daughter(0), genParts);
      daughter2 = 1 + getLatestIndexOfDaughters(part, genParts);
    }

    // Standard info
    std::cout << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // Print mothers
    std::cout << std::setw(8) << mother1 << "| " << std::setw(8) << mother2 << "| ";

    // Print daughters
    std::cout << std::setw(10) << daughter1 << "| " << std::setw(10) << daughter2 << "| ";

    // Particle properties
    std::cout << std::setw(13) << part.px() << "| " << std::setw(13) << part.py() << "| " << std::setw(13) << part.pz() << "| " << std::setw(13) << part.energy() << "| " << std::setw(13) << part.mass() << "\n";
   
    eventElement++;
  }
}

int GenSimEventDumpModule::getIndexOf(const reco::Candidate* part, const std::vector<reco::GenParticle>& genParts) const
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

int GenSimEventDumpModule::getLatestIndexOfMothers(const reco::GenParticle& part, const std::vector<reco::GenParticle>& genParts) const
{
  int indexOf = -1;
  for(int i = 0; i < (int) part.numberOfMothers(); i++)
  {
    int compareIndex = getIndexOf(part.mother(i), genParts);
    if(compareIndex >= indexOf)
    {
      indexOf = compareIndex;
    }
  }

  return indexOf;
}

int GenSimEventDumpModule::getLatestIndexOfDaughters(const reco::GenParticle& part, const std::vector<reco::GenParticle>& genParts) const
{
  int indexOf = -1;
  for(int i = 0; i < (int) part.numberOfDaughters(); i++)
  {
    int compareIndex = getIndexOf(part.daughter(i), genParts);
    if(compareIndex >= indexOf)
    {
      indexOf = compareIndex;
    }
  }

  return indexOf;
}

