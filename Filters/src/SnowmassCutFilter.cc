#include "CMSAnalysis/Filters/interface/SnowmassCutFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

std::string SnowmassCutFilter::getFilterString(const EventInput* inputMod) const
{
    auto leptons = inputMod->getLeptons(EventInput::RecoLevel::Reco).getParticles();
    for (unsigned i = 0; i < leptons.size(); ++i)
    {
        for (unsigned j = i + 1; j < leptons.size(); ++j)
        {
            if (!checkFlavor(leptons[i], leptons[j]) || !checkSign(leptons[i], leptons[j]))
            {
                continue;
            }
            if (!checkInvariantMass(leptons[i], leptons[j]))
            {
                return "";
            }
        }
    }
    return "Cut";
}

bool SnowmassCutFilter::checkInvariantMass(Particle p1, Particle p2) const
{
    // Calculate invariant mass
    auto total = p1.getFourVector() + p2.getFourVector();
    double invMass = total.M();
    return invMass > 12 && (invMass < 76 || invMass > 106);
}

bool SnowmassCutFilter::checkFlavor(Particle p1, Particle p2) const
{
    return p1.getType() == p2.getType() && (p1.getType() == ParticleType::electron() || p1.getType() == ParticleType::muon())
        && (p2.getType() == ParticleType::electron() || p2.getType() == ParticleType::muon());
}

bool SnowmassCutFilter::checkSign(Particle p1, Particle p2) const
{
    return p1.getCharge() != p2.getCharge();
}