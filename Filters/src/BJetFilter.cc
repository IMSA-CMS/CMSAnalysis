#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"


std::string BJetFilter::getFilterString(const InputModule* inputMod) const
{
    auto jets = inputMod->getJets(InputModule::RecoLevel::Reco);
    if(jets.getNumParticles() > 0)
    {
        return "";
    }
    else
    {
        return " ";
    }
}