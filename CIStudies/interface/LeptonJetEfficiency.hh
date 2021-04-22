#ifndef LEPTONJETEFFICIENCY_HH
#define LEPTONJETEFFICIENCY_HH

#include <memory>

#include "AnalysisModule.hh"

class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

class LeptonJetEfficiency : public AnalysisModule
{
public:
    LeptonJetEfficiency(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod);
    bool process(const edm::EventBase& event) override;
    void finalize() override;
    void writeAll() override {};

private:
    const std::shared_ptr<LeptonJetReconstructionModule> lepRecoMod;
    const std::shared_ptr<LeptonJetMatchingModule> lepMatchMod;

    int numRecoJets = 0;
    int numMatchedJets = 0;
};


#endif