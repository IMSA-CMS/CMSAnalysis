/* 
#ifndef NLEPTONJETSFILTER_HH
#define NLEPTONJETSFILTER_HH

#include <memory>
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"

class NLeptonJetsFilter : public FilterModule
{
public:
  NLeptonJetsFilter(std::shared_ptr<LeptonJetReconstructionModule> leptonJets);

protected:
  virtual std::string makeFilterString();
  std::shared_ptr<LeptonJetReconstructionModule> leptonJets;
};

#endif
*/