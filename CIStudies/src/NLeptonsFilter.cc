#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/Selector.hh"

NLeptonsFilter::NLeptonsFilter()
{}


std::string NLeptonsFilter::makeFilterString()
{
  int num = getInput()->getLeptons(InputModule::RecoLevel::Reco).getNumParticles(); 
  return std::to_string(num);
}
