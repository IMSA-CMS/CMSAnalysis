#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"


GenSimRecoPrototype::GenSimRecoPrototype(InputModule::RecoLevel type, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),  // Initialize the base class
  typeGenSim(type);
  //genSim(genSimModule),
  //reco(recoModule),
  // typeGenSim(typeSwitch)
{
}
