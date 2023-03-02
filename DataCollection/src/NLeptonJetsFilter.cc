#include "CMSAnalysis/DataCollection/interface/NLeptonJetsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include<iostream>

NLeptonJetsFilter::NLeptonJetsFilter(){};

std::string NLeptonJetsFilter::makeFilterString()
{
  int numberOfLeptonJets = getInput()->getJets(InputModule::RecoLevel::Reco).getNumParticles();
  std::cout<< "Number of Jets: " << numberOfLeptonJets;
  std::string returnString;
  if(numberOfLeptonJets == 0) {
    returnString = "No Jets";
  }
  else if(numberOfLeptonJets > 0){
    returnString = "Jet Detected";
  }
  else {
    throw std::runtime_error("Negative Jets");
  }
 
  return returnString;
};
