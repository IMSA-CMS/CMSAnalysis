#include "CMSAnalysis/Utility/interface/PileUpScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <vector>
#include <string>
#include <limits>

PileUpScaleFactor::PileUpScaleFactor(ParticleType particleType)
    : ScaleFactor("PileUpScaleFactor", nullptr), particleType(particleType)
{
}

std::vector<std::string> PileUpScaleFactor::getKey(const EventInput* input) const
{
    int pileUp = input->getNumPileUpInteractions();
    const auto& scaleFactorMap = getScaleFactorMap();
    for (const auto& entry : scaleFactorMap) {
        int pileUpKey = std::stoi(entry.first);

        if (pileUpKey > pileUp || pileUpKey == pileUp) {
            pileUp = pileUpKey;
        }
    }
    return {std::to_string(pileUp)};
}