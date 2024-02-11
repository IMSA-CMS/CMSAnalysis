#include "CMSAnalysis/Filters/interface/SameSignInvariantMassFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
SameSignInvariantMassFilter::SameSignInvariantMassFilter(double sameSignInvariantMassCut):
cut(sameSignInvariantMassCut)
{

}


std::string SameSignInvariantMassFilter::getFilterString(const EventInput* inputMod) const
{
    auto leptons = inputMod->getParticles(EventInput::RecoLevel::Reco);
    auto inv = leptons.calculateSameSignInvariantMass(false, true);
    if (inv < cut)
    {
        return "lowMass";
    } else
    {
        return "highMass";
    }
    return "Cut";
}