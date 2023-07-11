#include "CMSAnalysis/Histograms/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/Utility/interface/HistogramPrototype.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

ResolutionPrototype::ResolutionPrototype(const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum) //initialize the base class
{
}

std::vector<double> ResolutionPrototype::value() const
{
  auto genSimValue = getValue(getInput()->getLeptons(EventInput::RecoLevel::GenSim));
  //std::cerr << "genSimValue = " << genSimValue << std::endl;
  double recoValue = getValue(getInput()->getLeptons(EventInput::RecoLevel::Reco));
  //std::cerr << "recoValue = " << recoValue << std::endl;
  return {((recoValue - genSimValue)/genSimValue)};
}
