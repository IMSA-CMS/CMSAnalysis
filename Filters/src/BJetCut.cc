#include "CMSAnalysis/Filters/interface/BJetCut.hh"

#include <vector>

#include "CMSAnalysis/Utility/interface/Event.hh"


bool BJetCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const std::vector<Jet>& jets = event.getJets().getParticles();
    for (int i = 0; i < int(jets.size()); i++) 
    {
        if (jets[i].getInfo("bJet"))
        {
            return false;
        }
    }
    return true;
}