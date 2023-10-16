#include "CMSAnalysis/Filters/interface/HiggsMassCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

bool HiggsMassCut::checkEventInternal(const Event& event) const
{
    const auto particles = event.getParticles();

    if (particles.calculateSameSignInvariantMass(false, false) >= 1000)
    {
        return true;
    }

    return false;
}
