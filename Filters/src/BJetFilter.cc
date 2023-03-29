#include "CMSAnalysis/DataCollection/interface/BJetFilter.hh"


std::string BJetFilter::makeFilterString()
{
    auto jets = getInput()->getJets(InputModule::RecoLevel::Reco);
    if(jets.getNumParticles() > 0)
    {
        return "";
    }
    else
    {
        return " ";
    }
}