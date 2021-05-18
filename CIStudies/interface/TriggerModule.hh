#ifndef TRIGGERMODULE_HH
#define TRIGGERMODULE_HH

#include <memory>

#include "ProductionModule.hh"

class Trigger;
class RecoIdentificationModule;

class TriggerModule : public ProductionModule
{
public:
  TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod);
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;
  void addTrigger(std::shared_ptr<Trigger> trigger) {triggers.push_back(trigger);}
  void printComboEfficiency(int passCombo, std::vector<int> trigIndices);
  void printOverallEfficiency();

private:
  int twoTriggerOverlap(int index1, int index2);
  int nTriggerOverlap(std::vector<int> indices);
  void callTriggerCombo();
  void triggerCombinations(int offset, int nCombo, std::vector<int>& combination, std::vector<int>& nTriggers);
  void findMostEfficientCombo(double efficiency, int nPass, std::vector<int> combo);
  const std::shared_ptr<RecoIdentificationModule> recoMod;
  std::vector<std::shared_ptr<Trigger>> triggers;
  std::unordered_map<std::vector<bool>,int> overlap;
  std::vector<std::vector<int>> trigCombos;
  double highestEfficiency = 0;
  int highestPass = 0;
  std::vector<int> highestEffCombo;
  int passAny;
  int total;
};

#endif
