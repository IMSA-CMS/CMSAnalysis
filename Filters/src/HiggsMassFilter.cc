#include "CMSAnalysis/Filters/interface/HiggsMassFilter.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"

std::string HiggsMassFilter::getFilterString(const EventInput *const input) const
{
    const auto leptons = input->getLeptons(EventInput::RecoLevel::Reco);
    if (leptons.size() < 4)
    {
        return "LowMass";
    }
    const auto pair = Utility::findBestLeptonPairing(leptons);
    const auto maxMass = std::max(Particle::invariantMass(pair.first.first, pair.first.second),
                            Particle::invariantMass(pair.second.first, pair.second.second));
    if (maxMass > cutoff)
    {
        return "HighMass";
    }
    else
    {
        return "LowMass";
    }
}
