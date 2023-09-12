#ifndef HPLUSPLUSDECAYFILTER_HH
#define HPLUSPLUSDECAYFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

//Filters hpp events based upon their decays. Implemented for both GenSim and Reco (in progress)
class HPlusPlusDecayFilter : public Filter
{
public:
  HPlusPlusDecayFilter(InputModule::RecoLevel isGenSim);
  std::string getState(const InputModule* inputMod) const;

protected:
  InputModule::RecoLevel typeGenSim;
  static int getIndex(std::string* arr, std::string elem, int size);
  virtual std::string getFilterString(const InputModule* inputMod) const override; 
  };

#endif
