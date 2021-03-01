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
    LeptonJetMatchingModule(const GenSimParticleModule& genSimModule, const LeptonJetReconstructionModule& lepJetModule, double deltaRCut = 0.5);
    virtual bool process(const edm::EventBase& event) override;
    const std::vector<MatchingPair>& getMatchingPairs() const {return matchingPairs;}

private:
    double findMatchingPairDeltaR(MatchingPair pair);
    
    std::vector<MatchingPair> matchingPairs;
    const GenSimParticleModule& genSim;
    const LeptonJetReconstructionModule& lepJet;
    double deltaRCutoff;
};

#endif