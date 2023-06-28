#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"


std::string BJetFilter::getFilterString(const EventInput* inputMod) const
{
    auto jets = inputMod->getJets(EventInput::RecoLevel::Reco);
    if(jets.getNumParticles() > 0)
    {
        return "";
    }
    else
    {
        return " ";
    }
}