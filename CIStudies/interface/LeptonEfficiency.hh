#ifndef LEPTONEFFICIENCY_HH
#define LEPTONEFFICIENCY_HH

#include "EfficiencyModule.hh"

class MatchingModule;
//class GenSimIdentificationModule;

class LeptonEfficiency : public EfficiencyModule
{
public:
  LeptonEfficiency(const std::shared_ptr<WeightingModule> weightMod, const std::shared_ptr<MatchingModule> imatchModule);
  bool process() override;
  void finalize() override;
  void writeAll() override {}
private:
  const std::shared_ptr<MatchingModule> matchModule;
  //const std::shared_ptr<GenSimIdentificationModule> genSimModule;
  int recoMuons, genSimMuons, recoElectrons, genSimElectrons;

  protected:
  void doCounters() override{}
};

#endif
