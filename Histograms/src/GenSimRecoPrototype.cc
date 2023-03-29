#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/Histograms/interface/HistogramPrototype.hh"


GenSimRecoPrototype::GenSimRecoPrototype(InputModule::RecoLevel type, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)  // Initialize the base class
  //genSim(genSimModule),
  //reco(recoModule),
  // typeGenSim(typeSwitch)
{
  typeGenSim = type;
}
