#include "CMSAnalysis/Filters/interface/HiggsZVetoFilter.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

std::string HiggsZVetoFilter::getFilterString(const EventInput *input) const
{

    auto muons = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());
    auto electrons = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::electron());

    double mass;
    for (int i = 0; i < int(electrons.size()) - 1; i++)
    {
        for (int j = i + 1; j < int(electrons.size()); j++)
        {
            mass = (electrons[i].getFourVector() + electrons[j].getFourVector()).M();
            if (electrons[i].getCharge() != electrons[j].getCharge() && (71 < mass && mass < 111))
            {
                return "ZPeak";
            }
        }
    }

    for (int i = 0; i < int(muons.size()) - 1; i++)
    {
        for (int j = i + 1; j < int(muons.size()); j++)
        {

            mass = (muons[i].getFourVector() + muons[j].getFourVector()).M();
            if (muons[i].getCharge() != muons[j].getCharge() && (71 < mass && mass < 111))
            {
                return "ZPeak";
            }
        }
    }
    return "ZVeto";
}
