#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"

NLeptonsFilter::NLeptonsFilter(){};

std::string NLeptonsFilter::getFilterString(const InputModule* inputMod) const
{
  int num = inputMod->getLeptons(InputModule::RecoLevel::Reco).getNumParticles();
  return std::to_string(num);
};
