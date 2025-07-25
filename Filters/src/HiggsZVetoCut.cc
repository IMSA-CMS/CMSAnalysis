#include "CMSAnalysis/Filters/interface/HiggsZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <vector>

bool HiggsZVetoCut::checkEventInternal(const Event &event, const EventInput *input) const
{

    const std::vector<Muon> &muons = event.getMuons().getParticles();
    const std::vector<Electron> &electrons = event.getElectrons().getParticles();

    double mass;
    for (int i = 0; i < int(electrons.size()) - 1; i++)
    {
        for (int j = i + 1; j < int(electrons.size()); j++)
        {

            mass = (electrons[i].getFourVector() + electrons[j].getFourVector()).M();
            if (electrons[i].getCharge() != electrons[j].getCharge() && (mass > 71 && mass < 111))
            {
                return false;
            }
        }
    }

    for (int i = 0; i < int(muons.size()) - 1; i++)
    {

        for (int j = i + 1; j < int(muons.size()); j++)
        {

            mass = (muons[i].getFourVector() + muons[j].getFourVector()).M();
            if (muons[i].getCharge() != muons[j].getCharge() && (mass > 71 && mass < 111))
            {
                return false;
            }
        }
    }
    return true;
}
