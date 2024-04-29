#ifndef LeptonJetPTDifferential_HH
#define LeptonJetPTDifferential_HH

#include <memory>

#include "CMSAnalysis/Modules/interface/EfficiencyModule.hh"

#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

//Responsible for calculating the difference of PT among lepton jet particles
class LeptonJetPTDifferential : public EfficiencyModule 
{
public:
    LeptonJetPTDifferential(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod);
    
    void finalize() override;

protected:
    virtual void doCounters() override;

private:
    bool oppositeChargeLeptonJetFilter(LeptonJet &jet);
    const std::shared_ptr<LeptonJetReconstructionModule> lepRecoMod;
    const std::shared_ptr<LeptonJetMatchingModule> lepMatchMod;

    double pT_Difference[2] = {0.0, 0.0};
    int count = 0;
    int total_jets[2] = {0, 0};
    int total_jets_sizes[2] = {0, 0};
};


#endif
