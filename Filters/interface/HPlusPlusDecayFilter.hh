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
  std::string getState(const EventInput* inputMod) const;

protected:
  EventInput::RecoLevel typeGenSim;
  static int getIndex(std::string* arr, std::string elem, int size);
  virtual std::string getFilterString(const EventInput* inputMod) const override; 
  };

#endif
