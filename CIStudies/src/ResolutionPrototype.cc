#include "CIAnalysis/CIStudies/interface/ResolutionPrototype.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

ResolutionPrototype::ResolutionPrototype(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum), //initialize the base class
  genSim(genSimModule),
  reco(recoModule)
{
}

std::vector<double> ResolutionPrototype::value() const
{
  double genSimValue = getValue(genSim->getGenParticles());
  //std::cerr << "genSimValue = " << genSimValue << std::endl;
  double recoValue = getValue(reco->getRecoCandidates());
  //std::cerr << "recoValue = " << recoValue << std::endl;
  return {((recoValue - genSimValue)/genSimValue)};
}
