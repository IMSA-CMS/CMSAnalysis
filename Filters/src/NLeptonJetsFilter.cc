/*
#include "CMSAnalysis/Filters/interface/NLeptonJetsFilter.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include <iostream>

NLeptonJetsFilter::NLeptonJetsFilter(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule) : 
  leptonJets(lepJetModule)
{};

std::string NLeptonJetsFilter::makeFilterString()
{
  std::vector<LeptonJet> leptonJetsVector = leptonJets->getLeptonJets();
  int numberOfLeptonJets = leptonJetsVector.size();
  std::string returnString; 
  if(numberOfLeptonJets == 0) {
    returnString = "No Jets ";
  }
  else if(numberOfLeptonJets == 1){
    returnString = "1 Jets ";
  }
  else if(numberOfLeptonJets == 2){
    returnString = "2 Jets ";
  }
  else if(numberOfLeptonJets == 3){
    returnString = "3 Jets ";
  }
  else if(numberOfLeptonJets > 3){
    returnString = "4 or more Jets ";
  }
  else {
    throw std::runtime_error("Negative Jets");
  }
 
  return returnString;
};
//look at lepton jet matching module
//get number of jets from lepton jet reconstruction modeule
*/