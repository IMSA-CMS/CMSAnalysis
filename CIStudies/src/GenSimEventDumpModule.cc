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
  int motherColumnWidth = 20;
  int daughterColumnWidth = 20;

  // Format
  std::cerr << "--------------------------------------------------------" << std::endl;
  std::cerr << "EVENT #" << (counter + 1) << std::endl;
  std::cerr << "--------------------------------------------------------" << std::endl;

  std::cout << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
   << std::setw(10) << "| status"
   << std::setw(motherColumnWidth) << "| mothers"
   << std::setw(daughterColumnWidth) << "| daughters"
   << std::setw(15) << "| px"
   << std::setw(15) << "| py"
   << std::setw(15) << "| pz"
   << std::setw(15) << "| E"
   << std::setw(5) << "| mass\n";

  //Prints out all of the particles
  for(auto &part : genParts){
    
    // Event Elements
    std::string mothers = "";
    std::string daughters = "";

    for(int i = 0; i < (int) part.numberOfMothers(); i++)
    {
      mothers = mothers + (1 + getIndexOf(part.mother(i), genParts)) + " ";
    }

    // Debug
    //std::cout << part.numberOfMothers() << ", " << part.numberOfDaughters() << std::endl;

    for(int i = 0; i < (int) part.numberOfDaughters(); i++)
    {
      daughters = daughters + (1 + getIndexOf(part.daughter(i), genParts)) + " ";
    }

    // TODO: Write a method that formats daughter and mother output.
    // From 1 2 3 4 7 to 1-4 7

    // Standard info
    std::cout << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // Print mothers
    std::cout << std::setw(motherColumnWidth - 2) << mothers << "| ";

    // Print daughters
    std::cout << std::setw(daughterColumnWidth - 2) << daughters << "| ";

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

