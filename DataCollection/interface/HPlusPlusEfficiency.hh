#ifndef HPLUSPLUSEFFICIENCY_HH
#define HPLUSPLUSEFFICIENCY_HH

#include "EfficiencyModule.hh"

class MatchingModule;

//Responsible for calculating reconstruction efficiency for leptons
class HPlusPlusEfficiency : public EfficiencyModule
{
public:
  HPlusPlusEfficiency(const std::shared_ptr<MatchingModule> imatchModule);

  bool process() override;
  void finalize() override;
  void writeAll() override {}
  
private:
  const std::shared_ptr<MatchingModule> matchModule;

  //counters for ammount of reco and gensim leptons / these are used to calculate the efficiency
  int genSimMuons, genSimElectrons, genSimHPlusPlus;

  std::vector<Particle> doublyHiggs;
  std::vector<std::string> decays;
  

  protected:
  void doCounters() override{}
};

#endif