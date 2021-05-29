#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

NLeptonsFilter::NLeptonsFilter(const std::shared_ptr<RecoIdentificationModule> iRecoModule) :
  recoModule(iRecoModule)
{
}

std::string NLeptonsFilter::makeFilterString(const edm::EventBase& event)
{
  double pairs = recoModule->getRecoCandidates().getNumParticles();
  //std::cout << pairs.getSize() << "\n";
  return std::to_string(static_cast<int>(pairs));
}
