#include "CIAnalysis/CIStudies/interface/METModule.hh"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/Handle.h"

bool METModule::process(const edm::EventBase& event)
{
  edm::Handle<std::vector<pat::MET>> mets;
  event.getByLabel(std::string("slimmedMETs"), mets);

  std::cerr << "How much did the Mets score?" << mets->size() << std::endl;

  for (const auto& p : *mets)
  {
    //if (p.isCaloMET())
    //{
      met = p.corPt(pat::MET::METCorrectionLevel::Type1);
      phi = p.corPhi(pat::MET::METCorrectionLevel::Type1);
    //}
  }
  
  std::cerr << "met = " << met << std::endl;
  std::cerr << "phi = " << phi << std::endl;

  return true;
}
