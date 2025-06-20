#ifndef MATCHINGMODULE_HH
#define MATCHINGMODULE_HH

#include "ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/MatchingPairCollection.hh"

// MatchingModule creates a MatchingPairCollection by matching the gen sim particles with the reco particles
class MatchingModule : public ProductionModule
{
  public:
    MatchingModule(double deltaRCut = 0.1);
    bool process() override;

    const MatchingPairCollection &getMatchingBestPairs() const
    {
        return matchingBestPairs;
    }

    std::string getName() override
    {
        return "MatchingModule";
    }

  protected:
    bool match(std::vector<Particle> genSimparticles, std::vector<Particle> recoCandidates);

  private:
    // Matching fails if gensim/reco deltaR > deltaRCutoff.
    const double deltaRCutoff;

    MatchingPairCollection matchingBestPairs;
};

#endif
