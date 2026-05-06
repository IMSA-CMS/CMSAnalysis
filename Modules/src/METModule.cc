#include "CMSAnalysis/Modules/interface/METModule.hh"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/Handle.h"

bool METModule::process()
{

  auto metObj = getInput()->getMET();
  met = metObj.pt();
  phi = metObj.phi();

  return true;
};
