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
    LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut);
    virtual bool process() override;
    void finalize() override;
    const std::vector<std::pair<Particle, LeptonJet>> getMatchingPairs() const;
    const bool isQuark(GenSimParticle lepton);
    const bool isSquark(GenSimParticle lepton);
    const bool isHiggs(GenSimParticle lepton);

private:
    double findMatchingPairDeltaR(MatchingPair pair);

    std::vector<MatchingPair> matchingPairs;
    std::shared_ptr<GenSimParticleModule> genSim;
    std::shared_ptr<LeptonJetReconstructionModule> lepJet;
    double deltaRCutoff;

    int lepJetSize = 0;
    int genSize = 0;
    double darkPhoton = 0;
    int eventNumber = 1;
    int squark = 0;
    int quark = 0;
    int proton = 0;
    int genLeptons = 0;
    int darkPhotonOrigin = 0;
};

#endif