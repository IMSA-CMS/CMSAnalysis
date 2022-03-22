#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"

#include <cmath>
#include <stdexcept>
#include <fstream>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

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
    ParticleCollection genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim,Particle::Type::None,nullptr);
    //TODO this line needs to be fixed and put back in
    //Input Module is GenSim to match printGenParticleCollection
    if( getInput()->getLeptons(InputModule::RecoLevel::GenSim).getNumParticles() == 4)
    {
      std::ofstream my_file;
      my_file.open("EventDumpWWW.txt", std::ios::app);
      printGenParticleCollection(genParticles, my_file);
      //std::cout << "\nAn event was printed";
      my_file.close();
    }
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

void GenSimEventDumpModule::printGenParticleCollection(const ParticleCollection& genParts,std::ofstream& my_file ) const
{
  int eventElement = 1;
  int motherColumnWidth = 20;
  int daughterColumnWidth = 20;
  const std::vector<Particle>& particleGroup = genParts.getParticles();
  //if(counter>= 10){
    //return;
  //}
  // Format
  my_file << "--------------------------------------------------------" << std::endl;
  my_file << "EVENT #" << (counter + 1) << std::endl;
  my_file << "--------------------------------------------------------" << std::endl;

  my_file << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
   << std::setw(10) << "| status"
   << std::setw(motherColumnWidth) << "| mothers"
   << std::setw(daughterColumnWidth) << "| daughters"
   << std::setw(15) << "| pT"
   << std::setw(15) << "| Eta"
   << std::setw(15) << "| Phi"
   << std::setw(15) << "| E"
   << std::setw(5) << "| mass\n";

  //Prints out all of the particles
  for(auto &part : particleGroup){

    // Standard info
    my_file << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // fix formatMotherParticles function
    // Print mothers
    my_file << std::setw(motherColumnWidth - 2) << "0" << "| ";
    //my_file << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";

    // Print daughters
    my_file << std::setw(daughterColumnWidth - 2) << formatDaughterParticles(part, particleGroup) << "| ";

    // Particle properties
    my_file << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| " << std::setw(13) << part.energy() << "| " << std::setw(13) << part.getMass() << "\n";
    eventElement++;
    }
}

int GenSimEventDumpModule::getIndexOf(const Particle& part, const std::vector<Particle>& genParts) const
{
  int indexOf = 0;
  for(auto possiblePart : genParts)
  {
    if(possiblePart == part)
      return indexOf;
    indexOf++;
  }
  return -1;
}

int GenSimEventDumpModule::getLatestIndexOfDaughters(const Particle& part, const std::vector<Particle>& genParts) const
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

std::string GenSimEventDumpModule::formatDaughterParticles(const Particle& part, const std::vector<Particle>& genParts) const
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

