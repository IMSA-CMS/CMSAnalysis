#include "CMSAnalysis/DataCollection/interface/LeptonFilter.hh"

LeptonFilter::LeptonFilter(const ParticleType& itype, int inParticles, std::string ioutputString) :
    type(itype),
    nParticles(inParticles),
    outputString(ioutputString)
{}

std::string LeptonFilter::makeFilterString()
{
    auto leptons = getInput()->getLeptons(InputModule::RecoLevel::Reco).getParticles();

    int particleCount = 0;
    for (const auto &lep : leptons)
    {
        if (lep.getType() == type)
        {
            ++particleCount;
        }
    }
    return particleCount >= nParticles ? outputString : "";
};