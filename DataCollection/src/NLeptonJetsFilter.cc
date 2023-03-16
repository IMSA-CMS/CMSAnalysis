#include "CMSAnalysis/DataCollection/interface/NLeptonJetsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include<iostream>

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
  else if(numberOfLeptonJets > 0){
    returnString = "Jet Detected ";

  }
  else {
    throw std::runtime_error("Negative Jets");
  }
 
  return returnString;
};
//look at lepton jet matching module
//get number of jets from lepton jet reconstruction modeule