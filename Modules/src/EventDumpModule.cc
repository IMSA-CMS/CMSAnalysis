#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"

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

EventDumpModule::EventDumpModule(bool iGenSim, bool iReco, int inumOfEvents):
  genSim(iGenSim),
  reco(iReco),
  numOfEvents(inumOfEvents)
{
  counter = 0;
  //clears out the text files of current data
  std::ofstream my_file;
  my_file.open("GenSimEventDump.txt", std::ofstream::out | std::ofstream::trunc);
  my_file.close();
  my_file.open("RecoEventDump.txt", std::ofstream::out | std::ofstream::trunc);
  my_file.close();
}

//update this to remove event parameter
bool EventDumpModule::process()
{
  
  if(counter < numOfEvents || numOfEvents == -1)
  {
    //if (getFilter() == "eeeu_") 
    {
      std::ofstream my_file;
      if(genSim)
      {
        auto genParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none());
        my_file.open("GenSimEventDump.txt", std::ios::app);
        printGenSimParticleCollection(genParticles, my_file);
        //std::cout << "\nAn event was printed";
        my_file.close();
      }
      if(reco)
      {
        auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
        my_file.open("RecoEventDump.txt", std::ios::app);
        if (counter == 0)
        {
          my_file << std::left << std::setw(8) << "event" << std::setw(8) << "element" << std::setw(11) << "| name" << std::setw(11) << "| type"
          << std::setw(15) << "| charge"
          << std::setw(15) << "| pT"
          << std::setw(15) << "| Eta"
          << std::setw(15) << "| Phi"
          << std::setw(15) << "| E"
          << std::setw(5) << "| mass\n";
        }
        printRecoParticleCollection(recoParticles, my_file);
        //std::cout << "\nAn event was printed";
        my_file.close();
      }
      counter++;
      return true;
    }
  }
  return true;
}

void EventDumpModule::printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts,std::ofstream& my_file ) const
{
  int eventElement = 1;
  int motherColumnWidth = 20;
  int daughterColumnWidth = 20;
  const auto& particleGroup = genParts.getParticles();
  my_file << "--------------------------------------------------------" << std::endl;
  my_file << "EVENT #" << (counter + 1) <<std::endl;
  my_file << "--------------------------------------------------------" << std::endl;

  my_file << std::left << std::setw(11) << "element" << std::setw(15) << "| name" << std::setw(11) << "| pdgId"
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
    std::string partName = part.getType().getName();
    my_file << std::setw(11) << eventElement << "| " << std::setw(13) << partName << std::pair<GenSimParticle, std::vector<GenSimParticle>>{part, particleGroup} << std::endl;
    eventElement++;
  }
}

void EventDumpModule::printRecoParticleCollection(const ParticleCollection<Particle>& recoParts,std::ofstream& my_file ) const
{
  int eventElement = 1;
  const auto& particleGroup = recoParts.getParticles();
 // my_file << "--------------------------------------------------------" << std::endl;
 // my_file << "EVENT #" << (counter + 1) <<std::endl;
  //my_file << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass() 
  //  << " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass(true) << "\n";
 // my_file << "--------------------------------------------------------" << std::endl;



  //Prints out all of the particles
  for(auto &part : particleGroup)
  {
    //std::cout<<"Part: " << part.getType().getName() << "\n"; 
    const auto& partName = part.getType().getName();
    my_file << std::setw(8) << (counter + 1) << std::setw(8) << eventElement <<  "| " << std::setw(9) << partName<< part << std::endl;
    eventElement++;
  }
}

int EventDumpModule::getIndexOf(const Particle& part, const std::vector<Particle>& recoParts)
{
  int indexOf = 0;
  for(auto possiblePart : recoParts)
  {
    if(possiblePart == part)
      return indexOf;
    indexOf++;
  }
  return -1;
}

int EventDumpModule::getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts)
{
  int indexOf = 0;
  for(auto possiblePart : genParts)
  {
    if(possiblePart == part)
    {
      return indexOf;
    }  
    indexOf++;
  }
  return -1;
}

int EventDumpModule::getLatestIndexOfDaughters(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const
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

std::string EventDumpModule::formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts)
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

std::string EventDumpModule::formatMotherParticle(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts)
{
  
  //int index = getIndexOf(part.uniqueMother().finalDaughter(), genParts);
  int index = getIndexOf(part.mother(), genParts)+1;
  //std::cout<<"The index is: " << index << "\n";
  if (index == -1)
  {
    return "";
  }
  return std::to_string(index);
}

std::ostream& operator<<(std::ostream& str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles)
{
  GenSimParticle part = genParticles.first;
  //std::cout<<"The particles name is: " << part.getType().getName() << "\n";
  std::vector<GenSimParticle> genParts = genParticles.second;
  
  str << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

  //print mothers
  
  //std::cout<<"The mother is: " << part.hasMother().getName() << "\n";
    
  if(part.hasMother())
  { 
    //str << std::setw(18) << part.mother().getType().getName() << "| "; //this was original
    //str << std::setw(18) << EventDumpModule :: formatMotherParticle(part, genParts) << "| ";
    str << std::setw(8) << EventDumpModule :: formatMotherParticle(part, genParts);
    str <<std::setw(10) << part.mother().getType().getName() << "| ";
  }
  else
  {
    str << std::setw(18) << "" << "| ";
  }
    

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
  // str << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| "; 
    // Print daughters
    //formatDaughterParticles(part, particleGroup) was replaced by function not working string
  str << std::setw(18) << "" << "| ";
    // Particle properties
  str << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";
  str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();
  return str;
}
