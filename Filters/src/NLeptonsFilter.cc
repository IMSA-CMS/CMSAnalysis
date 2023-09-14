#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

NLeptonsFilter::NLeptonsFilter(){};

std::string NLeptonsFilter::getFilterString(const EventInput* inputMod) const
{
  int num = inputMod->getLeptons(EventInput::RecoLevel::Reco).getNumParticles();
  return std::to_string(num);
};
