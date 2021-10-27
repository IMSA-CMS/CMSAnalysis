#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

NLeptonsFilter::NLeptonsFilter()
{
}

std::string NLeptonsFilter::makeFilterString()
{
  double pairs = getInput()->getParticles(InputModule::RecoLevel::Reco).getNumParticles();  //recoModule->getRecoCandidates().getNumParticles();
  //std::cout << pairs.getSize() << "\n";
  return std::to_string(static_cast<int>(pairs));
}
