#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

NLeptonsFilter::NLeptonsFilter()
{
}

std::string NLeptonsFilter::makeFilterString()
{
  // std::cout << " Made it to NLeptonsFilter, calling getParticles from Input module\n";
  // std::cout << "N leptons:" << getInput() << "\n";
  //The below getParticles is causing the issue
  double pairs = getInput()->getParticles(InputModule::RecoLevel::Reco).getNumParticles();  //recoModule->getRecoCandidates().getNumParticles();
  //std::cout << pairs.getSize() << "\n";
  // std::cout << " Made it through NLeptonsFilter\n";
  return std::to_string(static_cast<int>(pairs));

}
