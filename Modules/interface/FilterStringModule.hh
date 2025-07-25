#ifndef FILTERSTRINGMODULE_HH
#define FILTERSTRINGMODULE_HH

#include "EfficiencyModule.hh"

//Responsible for calculating reconstruction efficiency for leptons
class FilterStringModule : public EfficiencyModule
{
public:
  FilterStringModule();

  bool process() override;
  void finalizeFilterString(TFile *outFile) override;
  void finalize() override{};

  virtual std::string getName() override {return "FilterStringModule";}
  
  protected:
  void doCounters() override{}
};

#endif
