#include "CMSAnalysis/DataCollection/interface/FilterModule.hh"

//update this to remove event parameter
bool FilterModule::process()
{
  filterString = makeFilterString();
  return !filterString.empty();
}
