#ifndef LEPTONJETMATCHINGMODULE_HH
#define LEPTONJETMATCHINGMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "GenSimParticleModule.hh"
#include "LeptonJetReconstructionModule.hh"

class LeptonJetMatchingModule : public ProductionModule
{
public:
    using MatchingPair = std::pair<Particle, LeptonJet>;
    LeptonJetMatchingModule(std::shared_ptr<GenSimParticleModule> genSimModule, std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut = 0.5);
    virtual bool process() override;
    void finalize() override;
    const std::vector<MatchingPair>& getMatchingPairs() const {return matchingPairs;}

private:
    double findMatchingPairDeltaR(MatchingPair pair);
    
    std::vector<MatchingPair> matchingPairs;
    std::shared_ptr<GenSimParticleModule> genSim;
    std::shared_ptr<LeptonJetReconstructionModule> lepJet;
    double deltaRCutoff;

    int lepJetSize = 0;
    int genSize = 0;
};

#endif