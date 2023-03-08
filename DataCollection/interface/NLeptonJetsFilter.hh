#ifndef NLEPTONJETSFILTER_HH
#define NLEPTONJETSFILTER_HH

#include <memory>
#include "FilterModule.hh"
#include "LeptonJetReconstructionModule.hh"

class NLeptonJetsFilter : public FilterModule
{
public:
  NLeptonJetsFilter(std::shared_ptr<LeptonJetReconstructionModule> leptonJets);

protected:
  virtual std::string makeFilterString() override;
  std::shared_ptr<LeptonJetReconstructionModule> leptonJets;
};

#endif
