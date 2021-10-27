#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"


GenSimRecoPrototype::GenSimRecoPrototype(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),  // Initialize the base class
  //genSim(genSimModule),
  //reco(recoModule),
  typeGenSim(typeSwitch)
{
}
