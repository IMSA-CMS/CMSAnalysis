#include "CMSAnalysis/Filters/interface/BJetCut.hh"

#include <vector>

#include "CMSAnalysis/Utility/interface/Event.hh"


bool BJetCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const auto jets = input->getJets(EventInput::RecoLevel::Reco).getParticles();
    for (int i = 0; i < int(jets.size()); i++) 
    {
        if (jets[i].getInfo("bJet") > 0.9)
        {
            return false;
        }
    }
    return true;
}