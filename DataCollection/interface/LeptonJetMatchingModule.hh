#ifndef LEPTONJETMATCHINGMODULE_HH
#define LEPTONJETMATCHINGMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "GenSimParticleModule.hh"
#include "MatchingModule.hh"
#include "LeptonJetReconstructionModule.hh"

class LeptonJetMatchingModule : public MatchingModule
{
public:
    using MatchingPair = std::pair<Particle, LeptonJet>;
    LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut = 0.5);
    virtual bool process() override;
    void finalize() override;
    const std::vector<std::pair<Particle,LeptonJet>> getMatchingPairs() const;

private:
    double findMatchingPairDeltaR(MatchingPair pair);
    
    std::vector<MatchingPair> matchingPairs;
    std::shared_ptr<GenSimParticleModule> genSim;
    std::shared_ptr<LeptonJetReconstructionModule> lepJet;
    double deltaRCutoff;

    int lepJetSize = 0;
    int genSize = 0;
    int darkPhoton = 0;
};

#endif