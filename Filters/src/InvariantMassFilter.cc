#include "CMSAnalysis/Filters/interface/InvariantMassFilter.hh"

InvariantMassFilter::InvariantMassFilter(double invariantMassCut, InputModule::RecoLevel inputLvl):
cut(invariantMassCut),
inputLevel(inputLvl)
{

}


std::string InvariantMassFilter::getFilterString(const InputModule* inputMod) const
{
    auto leptons =inputMod->getParticles(inputLevel);
    auto inv = leptons.getInvariantMass();
    if (inv < cut)
    {
        return "lowMass";
    } else
    {
        return "highMass";
    }
    return "Cut";
}