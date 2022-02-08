#include "CMSAnalysis/DataCollection/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

SnowmassLeptonSelector::SnowmassLeptonSelector(double iptCut, double iisoCut) :
    ptCut(iptCut),
    isoCut(iisoCut)
{}

bool SnowmassLeptonSelector::checkParticle(const Particle &particle) const
{
    if (particle.getPt() < ptCut || particle.getIsolation() > isoCut)
    {
        return false;
    }

    if (particle.getType() == Particle::Type::Electron)
    {
        return particle.getPt() > 10
        && std::abs(particle.getEta()) < 3;
    }
    else if (particle.getType() == Particle::Type::Muon)
    {
        return particle.getPt() > 4
        && std::abs(particle.getEta()) < 2.8;
    }
    else
    {
        return false;
    }
}
