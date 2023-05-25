#include <iostream>

#include "CMSAnalysis/DataCollection/interface/FilterStringModule.hh"

FilterStringModule::FilterStringModule():
  EfficiencyModule()
{
}

bool FilterStringModule::process()
{
  incrementCounter(getFilter(), 1);
  return true;
}

void FilterStringModule::finalizeFilterString()
{
  writeText(std::to_string(getCounter(getFilter())),getFilter());
}


