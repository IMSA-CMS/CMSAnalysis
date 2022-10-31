#include "CMSAnalysis/DataCollection/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

ResolutionPrototype::ResolutionPrototype(const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum) //initialize the base class
{
}

std::vector<double> ResolutionPrototype::value() const
{
  auto genSimValue = getValue(getInput()->getLeptons(InputModule::RecoLevel::GenSim));
  //std::cerr << "genSimValue = " << genSimValue << std::endl;
  double recoValue = getValue(getInput()->getLeptons(InputModule::RecoLevel::Reco));
  //std::cerr << "recoValue = " << recoValue << std::endl;
  return {((recoValue - genSimValue)/genSimValue)};
}
