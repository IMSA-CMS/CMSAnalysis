#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "CIAnalysis/CIStudies/interface/EventLoader.hh"
#include "CIAnalysis/CIStudies/interface/Selector.hh"

InputModule::InputModule(const EventLoader *iEventLoader) : eventLoader(iEventLoader)
{
}

ParticleCollection InputModule::getLeptons(RecoLevel level, std::shared_ptr<Selector> selector) const
{
    ParticleCollection leptons;
    auto chosenSelector = selector ? selector : leptonSelector;
    auto electrons = getParticles(level, Particle::Type::Electron).getParticles();
    auto muons = getParticles(level, Particle::Type::Muon).getParticles();
    for (const auto &p : electrons)
    {
        if (!chosenSelector || chosenSelector->checkParticle(p))
        {
            leptons.addParticle(p);
        }
    }
    for (const auto &p : muons)
    {
        if (!chosenSelector || chosenSelector->checkParticle(p))
        {
            leptons.addParticle(p);
        }
    }
    return leptons;
}

ParticleCollection InputModule::getParticles(RecoLevel level, Particle::Type particleType,  std::shared_ptr<Selector> selector) const
{
    ParticleCollection particleList;
    if (level == RecoLevel::GenSim)
    {
        auto particles = eventLoader->getFile()->getGenSimParticles().getParticles();
        for (const auto &p : particles)
        {
            if ((p.getType() == particleType || particleType == Particle::Type::None) && p.isFinalState())
            {
                if (!selector || selector->checkParticle(p))
                {
                    particleList.addParticle(p);
                }
            }
        }
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = eventLoader->getFile()->getRecoParticles().getParticles();
        for (const auto &p : particles)
        {
            if ((p.getType() == particleType || particleType == Particle::Type::None))
            {
                if (!selector || selector->checkParticle(p))
                {
                    particleList.addParticle(p);
                }
            }
        }
    }
    return particleList;
}

GenEventInfoProduct InputModule::getGenInfo() const
{
    return eventLoader->getFile()->getGenInfo();
}

double InputModule::getMET() const
{
    return eventLoader->getFile()->getMET();
}
// edm::TriggerResults InputModule::getTriggerResults(std::string subProcess) const
// {
//     return eventLoader->getTriggerResults(subProcess);
// }
// edm::TriggerNames InputModule::getTriggerNames(std::string subProcess) const
// {
//     return eventLoader->getTriggerNames(subProcess);
// }
