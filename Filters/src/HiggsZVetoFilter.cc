#include "CMSAnalysis/Filters/interface/HiggsZVetoFilter.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

std::string HiggsZVetoFilter::getFilterString(const EventInput *input) const
{
    auto leptons = input->getLeptons(EventInput::RecoLevel::Reco);

    for (int i = 0; i < int(leptons.size()) - 1; i++)
    {
        for (int j = i + 1; j < int(leptons.size()); j++)
        {
            if (leptons[i].getType() != leptons[j].getType()) // Have to be same flavor
            {
                continue;
            }
            if (leptons[i].getCharge() == leptons[j].getCharge()) // Have to be opposite charge
            {
                continue;
            }
            double mass = (leptons[i].getFourVector() + leptons[j].getFourVector()).M();
            if (71 < mass && mass < 111)
            {
                return "ZPeak";
            }
        }
    }
    return "ZVeto";
}
