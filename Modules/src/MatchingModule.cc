#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Utility/interface/MatchingPair.hh"

#include <cstddef>
#include <limits>
#include <utility>

MatchingModule::MatchingModule(double deltaRCut) : deltaRCutoff(deltaRCut)
{
}

bool MatchingModule::process()
{
    std::vector<Particle> genSimParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim).getParticles();
    std::vector<Particle> recoCandidates = getInput()->getParticles(EventInput::RecoLevel::Reco).getParticles();
    return match(genSimParticles, recoCandidates);
}

bool MatchingModule::match(std::vector<Particle> genSimParticles, std::vector<Particle> recoCandidates)
{
    matchingBestPairs.clear();

    while ((genSimParticles.size() > 0) && (recoCandidates.size() > 0))
    {
        // (gen, reco)
        std::pair<size_t, size_t> bestPair =
            std::pair(std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max());

        // Start with a high value, only really needs to be higher than the cutoff delta R
        double bestDeltaR = std::numeric_limits<double>::max();

        // Go through all possible particle combinations of gen and reco particles
        for (size_t genIdx = 0; genIdx < genSimParticles.size(); genIdx++)
        {
            for (size_t recoIdx = 0; recoIdx < recoCandidates.size(); recoIdx++)
            {
                MatchingPair candidate(GenSimParticle(genSimParticles.at(genIdx)), recoCandidates.at(recoIdx));
                auto deltaR = candidate.getDeltaR();
                if (deltaR < bestDeltaR)
                {
                    bestPair = std::pair(genIdx, recoIdx);
                    bestDeltaR = deltaR;
                }
            }
        }

        if (bestDeltaR > deltaRCutoff)
        {
            // Failed to match some particles
            return false;
        }

        matchingBestPairs.addMatchingPair(
            MatchingPair(GenSimParticle(genSimParticles.at(bestPair.first)), recoCandidates.at(bestPair.second)));

        genSimParticles.erase(genSimParticles.begin() + bestPair.first);
        recoCandidates.erase(recoCandidates.begin() + bestPair.second);
    }

    return true;
}
