#include <iostream>

#include "CMSAnalysis/Modules/interface/FilterStringModule.hh"

FilterStringModule::FilterStringModule():
  EfficiencyModule()
{
}

bool FilterStringModule::process()
{
  incrementCounter(getFilter(), 1);
  return true;
}

void FilterStringModule::finalizeFilterString(TFile *_outFile)
{
  writeText(std::to_string(getCounter(getFilter())),getFilter());
}


