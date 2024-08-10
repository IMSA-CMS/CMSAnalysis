#include "CMSAnalysis/Filters/interface/HighestMuonPtCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

bool HighestMuonPtCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const std::vector<Muon>& muons = event.getMuons().getParticles();

    double highestMuonPt = 0;

    for (auto muon : muons) {
        double pt = muon.getPt();

        if (pt > highestMuonPt)
        {
            highestMuonPt = pt;
        }
    }

    return (highestMuonPt > 27);
}
