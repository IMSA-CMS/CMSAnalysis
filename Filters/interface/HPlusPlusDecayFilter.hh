#ifndef HPLUSPLUSDECAYFILTER_HH
#define HPLUSPLUSDECAYFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

//Filters hpp events based upon their decays. Implemented for both GenSim and Reco (in progress)
class HPlusPlusDecayFilter : public Filter
{
public:
  HPlusPlusDecayFilter(EventInput::RecoLevel isGenSim);
  std::string getState(ParticleCollection<Particle> genSim, ParticleCollection<Particle> reco);

protected:
  EventInput::RecoLevel typeGenSim;
  virtual std::string getFilterString(const EventInput* inputMod) const override; 
  };

#endif
