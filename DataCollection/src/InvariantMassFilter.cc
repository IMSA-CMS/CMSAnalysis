#include "CMSAnalysis/DataCollection/interface/InvariantMassFilter.hh"

InvariantMassFilter::InvariantMassFilter(double invariantMassCut, InputModule::RecoLevel inputLvl):
cut(invariantMassCut),
inputLevel(inputLvl)
{

}


std::string InvariantMassFilter::makeFilterString()
{
    auto leptons = getInput()->getParticles(inputLevel);
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