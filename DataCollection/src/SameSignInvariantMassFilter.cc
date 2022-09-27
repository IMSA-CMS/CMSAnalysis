#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassFilter.hh"

SameSignInvariantMassFilter::SameSignInvariantMassFilter(double sameSignInvariantMassCut):
cut(sameSignInvariantMassCut)
{

}


std::string SameSignInvariantMassFilter::makeFilterString()
{
    auto leptons = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto inv = leptons.calculateSameSignInvariantMass();
    if (inv < cut)
    {
        return "";
    } else
    {
        return "Cut";
    }
    return "Cut";
}