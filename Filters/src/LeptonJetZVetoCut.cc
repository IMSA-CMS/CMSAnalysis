#include "CMSAnalysis/Filters/interface/LeptonJetZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <vector>

bool LeptonJetZVetoCut::checkEventInternal(const Event &event, const EventInput *_input) const
{
    const std::vector<Muon> &muons = event.getMuons().getParticles();

    for (int i = 0; i < int(muons.size()) - 1; i++)
    {
        const Muon &muon1 = muons.at(i);
        for (int j = i + 1; j < int(muons.size()); j++)
        {
            const Muon &muon2 = muons.at(j);
            const double mass = (muon1.getFourVector() + muon2.getFourVector()).M();
            if (muon1.getCharge() != muon2.getCharge() && (81 < mass && mass < 101))
            {
                return false;
            }
        }
    }
    return true;
}
