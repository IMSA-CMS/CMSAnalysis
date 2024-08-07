#include "CMSAnalysis/Filters/interface/LeptonFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

LeptonFilter::LeptonFilter(const ParticleType& itype, int inParticles, std::string ioutputString) :
    type(itype),
    nParticles(inParticles),
    outputString(ioutputString)
{}

std::string LeptonFilter::getFilterString(const EventInput* inputMod) const
{
    auto leptons = inputMod->getLeptons(EventInput::RecoLevel::Reco).getParticles();

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