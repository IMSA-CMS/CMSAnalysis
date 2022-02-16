#include "CMSAnalysis/DataCollection/interface/SnowmassCutFilter.hh"

std::string SnowmassCutFilter::makeFilterString()
{
    auto leptons = getInput()->getLeptons(InputModule::RecoLevel::Reco).getParticles();
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

bool SnowmassCutFilter::checkInvariantMass(Particle p1, Particle p2)
{
    // Calculate invariant mass
    auto total = p1.getFourVector() + p2.getFourVector();
    double invMass = total.M();
    return invMass > 12 && (invMass < 76 || invMass > 106);
}

bool SnowmassCutFilter::checkFlavor(Particle p1, Particle p2)
{
    return p1.getType() == p2.getType() && (p1.getType() == Particle::Type::Electron || p1.getType() == Particle::Type::Muon)
        && (p2.getType() == Particle::Type::Electron || p2.getType() == Particle::Type::Muon);
}

bool SnowmassCutFilter::checkSign(Particle p1, Particle p2)
{
    return p1.charge() != p2.charge();
}