#include "CIAnalysis/CIStudies/interface/SnowmassLeptonSelector.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"

SnowmassLeptonSelector::SnowmassLeptonSelector(double iptCut) :
    ptCut(iptCut)
{}

bool SnowmassLeptonSelector::checkParticle(const Particle &particle) const
{
    if (particle.getPt() < ptCut)
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
