
#include "CMSAnalysis/Filters/interface/NLeptonJetsFilter.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>



std::string NLeptonJetsFilter::getFilterString(const EventInput* input) const
{
  auto leptonJetsVector = input->getSpecial("leptonJet");
  int numberOfLeptonJets = leptonJetsVector.size();
  std::string returnString; 
  if(numberOfLeptonJets == 0) {
    returnString = "";
  }
  else if(numberOfLeptonJets == 1){
    returnString = "1Jet";

  }
  else {
	returnString = "multiJet";
  }
  
 
  return returnString;
};
// //look at lepton jet matching module
// //get number of jets from lepton jet reconstruction modeule
