#ifndef LEPTONEFFICIENCY_HH
#define LEPTONEFFICIENCY_HH

#include "EfficiencyModule.hh"

class MatchingModule;

//Responsible for calculating reconstruction efficiency for leptons
class LeptonEfficiency : public EfficiencyModule
{
public:
  LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule);

  bool process() override;
  void finalize() override;
  void writeAll() override {}
  
private:
  const std::shared_ptr<MatchingModule> matchModule;

  //counters for ammount of reco and gensim leptons / these are used to calculate the efficiency
  int recoMuons, genSimMuons, recoElectrons, genSimElectrons;

  protected:
  void doCounters() override{}
};

#endif
