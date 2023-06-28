#include "CMSAnalysis/Filters/interface/SameSignInvariantMassFilter.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
SameSignInvariantMassFilter::SameSignInvariantMassFilter(double sameSignInvariantMassCut):
cut(sameSignInvariantMassCut)
{

}


std::string SameSignInvariantMassFilter::getFilterString(const InputModule* inputMod) const
{
    auto leptons = inputMod->getParticles(InputModule::RecoLevel::Reco);
    auto inv = leptons.calculateSameSignInvariantMass();
    if (inv < cut)
    {
        return "lowMass";
    } else
    {
        return "highMass";
    }
    return "Cut";
}