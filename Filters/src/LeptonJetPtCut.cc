#include "CMSAnalysis/Filters/interface/LeptonJetPtCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

bool LeptonJetPtCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const auto particles = event.getParticles();

    auto lepton = particles.getLeadingPtLepton();

    if (lepton.getType() == ParticleType::electron())
    {
        if (lepton.getPt() > 32)
        {
            return true;
        }
    }
    else if (lepton.getType() == ParticleType::muon())
    {
        if (lepton.getPt() > 29)
        {
            return true;
        }
    }

    return false;
}
