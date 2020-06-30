#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

GenSimRecoPrototype::GenSimRecoPrototype(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const bool typeSwitch, std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum),  // Initialize the base class
  genSim(genSimModule),
  reco(recoModule),
  typeGenSim(typeSwitch)
{
}

