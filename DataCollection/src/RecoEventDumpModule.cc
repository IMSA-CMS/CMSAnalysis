#include "CMSAnalysis/DataCollection/interface/RecoEventDumpModule.hh"

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

RecoEventDumpModule::RecoEventDumpModule(int inumOfEvents):
  numOfEvents(inumOfEvents)
{
  counter = 0;
  selectedcounter = 0;
}

//update this to remove event parameter
bool clearlatche = true;
bool RecoEventDumpModule::process()
{
  std::ofstream my_file;
  if (clearlatche) 
  {
    my_file.open("RecoEventDump.txt", std::ofstream::out | std::ofstream::trunc);
    clearlatche = false;
  }
  
  if(counter < numOfEvents || numOfEvents == -1)
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::None);
    //TODO this line needs to be fixed and put back in
    //Input Module is Reco to match printRecoParticleCollection
    // if( getInput()->getLeptons(InputModule::RecoLevel::Reco).getNumParticles() == 4)
    if (getInput()->getLeptons(InputModule::RecoLevel::Reco).calculateSameSignInvariantMass()>500)
    {
      my_file.open("RecoEventDump.txt", std::ios::app);
      printRecoParticleCollection(recoParticles, my_file);
    //   std::cout << "A reco event was printed\n";
    }
    counter++;
    my_file.close();
    return true;
  }
  else
  {
    my_file.close();
    return true;
  }
}

void RecoEventDumpModule::writeAll()
{
}


void RecoEventDumpModule::printRecoParticleCollection(const ParticleCollection<Particle>& recoParts,std::ofstream& my_file ) const
{
  //std::cout << "printing reco event \n";
  int eventElement = 1;
  const auto& particleGroup = recoParts.getParticles();
  //if(counter>= 10){
    //return;
  //}
  // Format
  my_file << "--------------------------------------------------------" << std::endl;
  my_file << "EVENT #" << (counter + 1) <<std::endl;
  my_file << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass() 
    << " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass() << "\n";
  my_file << "--------------------------------------------------------" << std::endl;

  my_file << std::left << std::setw(8) << "element" << std::setw(11) << "| type"
   << std::setw(15) << "| charge"
   << std::setw(15) << "| pT"
   << std::setw(15) << "| Eta"
   << std::setw(15) << "| Phi"
   << std::setw(15) << "| E"
   << std::setw(5) << "| mass\n";
  //Prints out all of the particles
  for(auto &part : particleGroup){

    // Standard info
    my_file << std::setw(8) << eventElement << "| " << std::setw(9) << int(part.getType()) << "| ";


    // Particle properties
    my_file << std::setw(13) << part.getCharge() << "| " << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";
    try{
      my_file << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass() << "\n";
    }
    catch(const std::exception& e){
      my_file << std::setw(13) << "N/A" << "| " << std::setw(13) << part.getMass() << "\n";
    }
    eventElement++;
    }
}

int RecoEventDumpModule::getIndexOf(const Particle& part, const std::vector<Particle>& recoParts) const
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


