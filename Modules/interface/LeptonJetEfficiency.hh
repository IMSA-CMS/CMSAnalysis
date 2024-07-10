#ifndef LEPTONJETEFFICIENCY_HH
#define LEPTONJETEFFICIENCY_HH

#include <memory>

#include "CMSAnalysis/Modules/interface/EfficiencyModule.hh"

class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

//Responsible for calculating the efficiency of Lepton Jet Matching
class LeptonJetEfficiency : public EfficiencyModule 
{
public:
    LeptonJetEfficiency(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod);
    
    void finalize() override;

    virtual std::string getName() override {return "LeptonJetEfficiency";}

protected:
    virtual void doCounters() override;

private:
    const std::shared_ptr<LeptonJetReconstructionModule> lepRecoMod;
    const std::shared_ptr<LeptonJetMatchingModule> lepMatchMod;

    int numRecoJets = 0;
    int numMatchedJets = 0;
    int recoZeroAnyVeto = 0;
    int recoZeroAllVeto = 0;
};


#endif
