#include "CMSAnalysis/Histograms/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/Histograms/interface/HistogramPrototype.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"

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
