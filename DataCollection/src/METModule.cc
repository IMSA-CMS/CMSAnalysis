#include "CMSAnalysis/DataCollection/interface/METModule.hh"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/Handle.h"

bool METModule::process()
{
  met = getInput()->getMET();

  return true;
};
