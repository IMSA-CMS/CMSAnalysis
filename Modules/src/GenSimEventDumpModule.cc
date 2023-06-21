#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"

#include <cmath>
#include <stdexcept>
#include <fstream>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

GenSimEventDumpModule::GenSimEventDumpModule(int inumOfEvents):
  numOfEvents(inumOfEvents)
{
  counter = 1;
}

bool GenSimEventDumpModule::process()
{
  std::ofstream my_file;
  if (clearlatch) 
  {
    my_file.open("GenSimEventDump.txt", std::ofstream::out | std::ofstream::trunc);
    my_file.close();
    clearlatch = false;
  }
  
  if(counter < numOfEvents || numOfEvents == -1)
  {
    auto genParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none());
    my_file.open("GenSimEventDump.txt", std::ios::app);
    printGenSimParticleCollection(genParticles, my_file);
    my_file.close();
    counter++;
    return true;
  }
  else
  {
    return true;
  }
}

void GenSimEventDumpModule::printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts,std::ofstream& my_file) const
{
  int eventElement = 1;
  int motherColumnWidth = 20;
  int daughterColumnWidth = 20;
  const auto& particleGroup = genParts.getParticles();
 
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

  for(auto &part : particleGroup)
  {
    my_file << std::setw(8) << eventElement << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    try{
      my_file << std::setw(motherColumnWidth - 2) << formatMotherParticle(part, particleGroup) << "| ";
    }
    catch(const std::exception& e){
      my_file << std::setw(motherColumnWidth - 2) << "Not Available"<< "| ";
    }

    try{
      my_file << std::setw(daughterColumnWidth - 2) << formatDaughterParticles(part, particleGroup) << "| ";
    }
    catch(const std::exception& e){
      my_file << std::setw(daughterColumnWidth - 2) << "Not Available"<< "| ";
    }

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

std::string GenSimEventDumpModule::formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
{
  std::string daughters = "";

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