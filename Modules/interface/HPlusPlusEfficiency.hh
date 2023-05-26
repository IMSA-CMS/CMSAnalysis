#ifndef HPLUSPLUSEFFICIENCY_HH
#define HPLUSPLUSEFFICIENCY_HH

#include "EfficiencyModule.hh"

//Calculates reconstruction efficiency for H++
class HPlusPlusEfficiency : public EfficiencyModule
{
public:
  HPlusPlusEfficiency();

  bool process() override;
  void finalize() override;
  
private:

  //counters for ammount of reco and gensim leptons / these are used to calculate the efficiency
  //int genSimMuons, genSimElectrons, genSimHPlusPlus;

  void doCounters() override{}
};

#endif