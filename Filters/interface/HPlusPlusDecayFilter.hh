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
  std::string getState(ParticleCollection<Particle> genSim, ParticleCollection<Particle> reco);

protected:
  InputModule::RecoLevel typeGenSim;
  virtual std::string getFilterString(const InputModule* inputMod) const override; 
  };

#endif
