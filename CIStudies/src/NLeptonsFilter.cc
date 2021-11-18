#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

NLeptonsFilter::NLeptonsFilter(double pTCut):
pTMin(pTCut)
{

}

std::string NLeptonsFilter::makeFilterString()
{
  double pairs = getInput()->getLeptons(InputModule::RecoLevel::Reco, pTMin).getNumParticles();  //recoModule->getRecoCandidates().getNumParticles();
  //std::cout << pairs.getSize() << "\n";
  // std::cout << " Made it through NLeptonsFilter\n";
  return std::to_string(static_cast<int>(pairs));

}
