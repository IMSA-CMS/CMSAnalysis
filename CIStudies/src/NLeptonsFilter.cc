#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/Selector.hh"

NLeptonsFilter::NLeptonsFilter(std::shared_ptr<Selector> selector) :
  leptonSelector(selector)
{}


std::string NLeptonsFilter::makeFilterString()
{
  double pairs = getInput()->getLeptons(InputModule::RecoLevel::Reco, leptonSelector).getNumParticles(); 
  return std::to_string(static_cast<int>(pairs));
}
