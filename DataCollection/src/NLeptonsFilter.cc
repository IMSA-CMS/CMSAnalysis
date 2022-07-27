#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"

NLeptonsFilter::NLeptonsFilter()
{}


std::string NLeptonsFilter::makeFilterString()
{
  int num = getInput()->getLeptons(InputModule::RecoLevel::GenSim).getNumParticles(); 
  return std::to_string(num);
}
