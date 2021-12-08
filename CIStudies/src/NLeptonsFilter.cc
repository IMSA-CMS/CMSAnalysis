#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"

std::string NLeptonsFilter::makeFilterString()
{
  double pairs = getInput()->getLeptons(InputModule::RecoLevel::Reco).getNumParticles(); 
  return std::to_string(static_cast<int>(pairs));
}
