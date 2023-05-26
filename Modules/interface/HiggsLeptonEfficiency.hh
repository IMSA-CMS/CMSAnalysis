#ifndef HIGGSLEPTONEFFICIENCY_HH
#define HIGGSLEPTONEFFICIENCY_HH

#include "EfficiencyModule.hh"

class MatchingModule;

//Responsible for calculating reconstruction efficiency for leptons
class HiggsLeptonEfficiency : public EfficiencyModule
{
public:
  HiggsLeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule);

  bool process() override;
  void finalize() override;
  
private:
  const std::shared_ptr<MatchingModule> matchModule;

  protected:
  void doCounters() override{}
};

#endif
