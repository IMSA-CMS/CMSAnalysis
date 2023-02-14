#ifndef HPLUSPLUSDECAYFILTER_HH
#define HPLUSPLUSDECAYFILTER_HH

#include "FilterModule.hh"

//Filters hpp events based upon their decays. Implemented for both GenSim and Reco (in progress)
class HPlusPlusDecayFilter : public FilterModule
{
public:
  HPlusPlusDecayFilter(InputModule::RecoLevel isGenSim);

protected:
  virtual std::string makeFilterString() override; 
  InputModule::RecoLevel typeGenSim;
  };

#endif
