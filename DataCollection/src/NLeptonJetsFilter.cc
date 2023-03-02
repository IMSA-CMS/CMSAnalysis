#include "CMSAnalysis/DataCollection/interface/NLeptonJetsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include<iostream>

NLeptonJetsFilter::NLeptonJetsFilter(){};

std::string NLeptonJetsFilter::makeFilterString()
{
  int numberOfLeptonJets = getInput()->getJets(InputModule::RecoLevel::GenSim).getNumParticles();
  std::cout<< "Number of Jets: " << numberOfLeptonJets;

  return std::to_string(numberOfLeptonJets);
};
