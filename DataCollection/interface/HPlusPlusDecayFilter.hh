#ifndef HPLUSPLUSDECAYFILTER_HH
#define HPLUSPLUSDECAYFILTER_HH

#include "FilterModule.hh"

//Filters hpp events based upon their decays. Currently only work for GenSim
class HPlusPlusDecayFilter : public FilterModule
{
public:
  HPlusPlusDecayFilter();

protected:
  virtual std::string makeFilterString() override; 

};

#endif
