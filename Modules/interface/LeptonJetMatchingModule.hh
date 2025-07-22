#ifndef LEPTONJETMATCHINGMODULE_HH
#define LEPTONJETMATCHINGMODULE_HH

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/MatchingPair.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "MatchingModule.hh"
#include "LeptonJetReconstructionModule.hh"

class LeptonJetMatchingModule : public MatchingModule
{
  public:
    LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut);
    bool process() override;
    void finalize() override;
    const std::vector<std::pair<Particle, LeptonJet>> getMatchingPairs() const
    {
        const auto pairs = getMatchingBestPairs().getPairs();
        std::vector<std::pair<Particle, LeptonJet>> out{};
        std::transform(pairs.cbegin(), pairs.cend(), std::back_inserter(out), [](MatchingPair pair) {
            return std::pair(pair.getGenParticle(), LeptonJet(pair.getRecoParticle()));
        });
        return out;
    };
    const bool isQuark(GenSimParticle lepton) const;
    const bool isSquark(GenSimParticle lepton) const;
    const bool isHiggs(GenSimParticle lepton) const;

    std::string getName() override
    {
        return "LeptonJetMatchingModule";
    }

  private:
    std::shared_ptr<LeptonJetReconstructionModule> lepJet;
    const double deltaRCutoff;

    int lepJetSize = 0;
    int genSize = 0;
    double darkPhoton = 0;
    int squark = 0;
    int quark = 0;
    int proton = 0;
    int genLeptons = 0;
    int darkPhotonOrigin = 0;
};

#endif