#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"

GenSimEventDumpModule::GenSimEventDumpModule(int inumOfEvents):
  numOfEvents(inumOfEvents)
{
  counter = 0;
}

//update this to remove event parameter
bool GenSimEventDumpModule::process()
{
  if(counter < numOfEvents || numOfEvents == -1)
  {
    ParticleCollection genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);

    //TODO this line needs to be fixed and put back in
    //printGenParticleCollection(genParticles);
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

    // Debug
    //std::cout << part.numberOfMothers() << ", " << part.numberOfDaughters() << std::endl;

    // Standard info
    std::cout << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // Print mothers
    std::cout << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";

    // Print daughters
    std::cout << std::setw(daughterColumnWidth - 2) << formatDaughterParticles(part, genParts) << "| ";

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

std::string GenSimEventDumpModule::formatMotherParticles(const reco::GenParticle& part, const std::vector<reco::GenParticle>& genParts) const
{
  std::string mothers = "";

  int motherIndexes [(int) part.numberOfMothers()];

  // Store all indexes
  for(int i = 0; i < (int) part.numberOfMothers(); i++)
  {
    motherIndexes[i] = getIndexOf(part.mother(i), genParts);
  }

  // Create formatted string based on indexes
  for(int i = 0; i < (int) part.numberOfMothers(); i++)
  {
    int start = i;
    int end = start;

    int x = start;
    if(start < ((int) part.numberOfMothers() - 1))
    {
       while(motherIndexes[x] + 1 == motherIndexes[x + 1] && (x != ((int) part.numberOfMothers() - 1)))
      {
        x++;
        i++;
        end = x;
      }
    }
      
    if(start != end)
    {
      mothers = mothers + (1 + getIndexOf(part.mother(start), genParts)) + "-" + (1 + getIndexOf(part.mother(end), genParts)) + " ";
    }
    else
    {
      mothers = mothers + (1 + getIndexOf(part.mother(start), genParts)) + " ";
    }
  }

  return mothers;
}

std::string GenSimEventDumpModule::formatDaughterParticles(const reco::GenParticle& part, const std::vector<reco::GenParticle>& genParts) const
{
  std::string daughters = "";

  int daughterIndexes [(int) part.numberOfDaughters()];

  // Store all indexes
  for(int i = 0; i < (int) part.numberOfDaughters(); i++)
  {
    daughterIndexes[i] = getIndexOf(part.daughter(i), genParts);
  }

  // Create formatted string based on indexes
  for(int i = 0; i < (int) part.numberOfDaughters(); i++)
  {
    int start = i;
    int end = start;

    int x = start;
    if(start < ((int) part.numberOfDaughters() - 1))
    {
       while(daughterIndexes[x] + 1 == daughterIndexes[x + 1] && (x != ((int) part.numberOfDaughters() - 1)))
      {
        x++;
        i++;
        end = x;
      }
    }
      
    if(start != end)
    {
      daughters = daughters + (1 + getIndexOf(part.daughter(start), genParts)) + "-" + (1 + getIndexOf(part.daughter(end), genParts)) + " ";
    }
    else
    {
      daughters = daughters + (1 + getIndexOf(part.daughter(start), genParts)) + " ";
    }
  }

  return daughters;
}

