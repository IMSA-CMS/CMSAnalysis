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
  std::ofstream my_file;
  if (clearlatch) 
  {
    my_file.open("GenSimEventDump.txt", std::ofstream::out | std::ofstream::trunc);
    clearlatch = false;
  }
  
  if(counter < numOfEvents || numOfEvents == -1)
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none());
    //TODO this line needs to be fixed and put back in
    //Input Module is GenSim to match printGenSimParticleCollection
    // if( getInput()->getLeptons(InputModule::RecoLevel::GenSim).getNumParticles() == 4)
    my_file.open("GenSimEventDump.txt", std::ios::app);
    printGenSimParticleCollection(genParticles, my_file);
    //std::cout << "\nAn event was printed";
    my_file.close();
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

void GenSimEventDumpModule::printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts,std::ofstream& my_file) const
{
  int eventElement = 1;
  int motherColumnWidth = 20;
  int daughterColumnWidth = 20;
  const auto& particles = genParts.getParticles();
  std::vector<GenSimParticle> particleGroup;
  for (auto& part : particles)
  {
    if (part == part.finalDaughter())
    {
      particleGroup.push_back(part);
    }
  }

  // Format
  my_file << "--------------------------------------------------------" << std::endl;
  my_file << "EVENT #" << (counter) <<std::endl;
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
  for(auto &part : particleGroup)
  {
    my_file << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // fix formatMotherParticles function
    // Print mothers
    //my_file << std::setw(motherColumnWidth - 2) << "0" << "| ";
    try{
      my_file << std::setw(motherColumnWidth - 2) << formatMotherParticle(part, particleGroup) << "| ";
    }
    catch(const std::exception& e){
      my_file << std::setw(motherColumnWidth - 2) << "Not Available"<< "| ";
    }
    // Print daughters
    //formatDaughterParticles(part, particleGroup) was replaced by function not working string
    try{
      my_file << std::setw(daughterColumnWidth - 2) << formatDaughterParticles(part, particleGroup) << "| ";
    }
    catch(const std::exception& e){
      my_file << std::setw(daughterColumnWidth - 2) << "Not Available"<< "| ";
    }
    // Particle properties
    my_file << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";
    try{
      my_file << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass() << "\n";
    }
    catch(const std::exception& e){
      my_file << std::setw(13) << "N/A" << "| " << std::setw(13) << part.getMass() << "\n";
    }
    eventElement++;
  }
}


int GenSimEventDumpModule::getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
{
  int indexOf = 1;
  for(auto possiblePart : genParts)
  {
    if(possiblePart == part)
      return indexOf;
    indexOf++;
  }
  return -1;
}

int GenSimEventDumpModule::getLatestIndexOfDaughters(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
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

std::string GenSimEventDumpModule::formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
{
  std::string daughters = "";

  // Store all indexes
  for(int i = 0; i < (int) part.numberOfDaughters(); i++)
  {
    int index = getIndexOf(part.daughter(i).finalDaughter(), genParts);
    if (index != -1)
    {
      daughters = daughters + index + " ";
    }
  }
  return daughters;
}
std::string GenSimEventDumpModule::formatMotherParticle(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
{
  int index = getIndexOf(part.uniqueMother().finalDaughter(), genParts);
  if (index == -1)
  {
    return "";
  }
  return std::to_string(index);
}