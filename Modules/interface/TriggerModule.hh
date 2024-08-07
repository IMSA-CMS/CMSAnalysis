#ifndef TRIGGERMODULE_HH
#define TRIGGERMODULE_HH

#include <memory>

#include "AnalysisModule.hh"
#include "CMSAnalysis/Filters/interface/Trigger.hh"

class TriggerModule : public AnalysisModule
{
public:
  TriggerModule();
  virtual bool process() override;
  virtual void finalize() override;
  void addTrigger(std::shared_ptr<Trigger> trigger) {triggers.push_back(trigger);}
  void printComboEfficiency(int passCombo, std::vector<int> trigIndices); //given a set of triggers, find the combined efficiency
  void printOverallEfficiency(); //print out a summary of most efficient combination, total events, total events passing one trigger, and overall efficiency

  virtual std::string getName() override {return "TriggerModule";}
private:
  int twoTriggerOverlap(int index1, int index2);
  int nTriggerOverlap(std::vector<int> indices);
  void callTriggerCombo();
  void triggerCombinations(int offset, int nCombo, std::vector<int>& combination, std::vector<int>& nTriggers);
  void findMostEfficientCombo(double efficiency, int nPass, std::vector<int> combo);
  std::vector<std::shared_ptr<Trigger>> triggers;
  std::unordered_map<std::vector<bool>,int> overlap;
  std::vector<std::vector<int>> trigCombos;
  double highestEfficiency = 0;
  int highestPass = 0;
  std::vector<int> highestEffCombo;
  int passAny;
  int total;
  std::map<std::string, int> passedFilterCount;
  std::map<std::string, int> totalFilterCount;
};

#endif
