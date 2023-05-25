#ifndef HPLUSPLUSDECAYFILTER_HH
#define HPLUSPLUSDECAYFILTER_HH

#include "FilterModule.hh"

//Filters hpp events based upon their decays. Implemented for both GenSim and Reco (in progress)
class HPlusPlusDecayFilter : public FilterModule
{
public:
  HPlusPlusDecayFilter(InputModule::RecoLevel isGenSim);
  std::string getState(ParticleCollection<Particle> genSim, ParticleCollection<Particle> reco);

protected:
  InputModule::RecoLevel typeGenSim;
  virtual std::string makeFilterString() override; 
  };

#endif
