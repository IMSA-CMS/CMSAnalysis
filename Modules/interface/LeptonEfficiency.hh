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

  virtual std::string getName() override {return "LeptonEfficiency";}
  
private:
  const std::shared_ptr<MatchingModule> matchModule;

  protected:
  void doCounters() override{}
};

#endif
