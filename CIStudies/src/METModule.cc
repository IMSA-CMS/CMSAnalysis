#include "CIAnalysis/CIStudies/interface/METModule.hh"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/Handle.h"

bool METModule::process(const edm::EventBase& event)
{
  edm::Handle<std::vector<pat::MET>> mets;
  event.getByLabel(std::string("slimmedMETs"), mets);

  for (const auto& p : *mets)
  {
    met = p.corPt(pat::MET::METCorrectionLevel::Type1);
    phi = p.corPhi(pat::MET::METCorrectionLevel::Type1);
  }

  return true;
}


