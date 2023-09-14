#include "CMSAnalysis/Filters/interface/InvariantMassFilter.hh"

InvariantMassFilter::InvariantMassFilter(double invariantMassCut, EventInput::RecoLevel inputLvl):
cut(invariantMassCut),
inputLevel(inputLvl)
{

}


std::string InvariantMassFilter::getFilterString(const EventInput* inputMod) const
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