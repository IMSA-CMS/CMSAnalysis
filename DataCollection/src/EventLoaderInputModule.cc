#include "CMSAnalysis/DataCollection/interface/EventLoaderInputModule.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"

EventLoaderInputModule::EventLoaderInputModule(const EventLoader *iEventLoader):
eventLoader(iEventLoader)
{
}

ParticleCollection<Lepton> EventLoaderInputModule::getLeptons(RecoLevel level) const
{
    ParticleCollection<Lepton> leptons;
    auto electrons = getParticles(level, Particle::Type::Electron).getParticles();
    auto muons = getParticles(level, Particle::Type::Muon).getParticles();
    for (const auto &p : electrons)
    {
        leptons.addParticle(p);
    }
    for (const auto &p : muons)
    {
        leptons.addParticle(p);
    }
    return leptons;
}

ParticleCollection<Particle> EventLoaderInputModule::getParticles(RecoLevel level, Particle::Type particleType) const
{
    ParticleCollection<Particle> particleList;
    if (level == RecoLevel::GenSim)
    {
        auto particles = eventLoader->getFile()->getGenSimParticles().getParticles();
        for (const auto &p : particles)
        {
            if ((p.getType() == particleType || particleType == Particle::Type::None) && p.isFinalState())
            {
                particleList.addParticle(p);
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
                particleList.addParticle(p);
            }
        }
    }
    return particleList;
}

/* TODO: getJets */
ParticleCollection<Particle> EventLoaderInputModule::getJets(RecoLevel level) const
{
    ParticleCollection<Particle> particleList;
    if (level == RecoLevel::GenSim)
    {
        throw std::runtime_error("GenSim Jets not implemented");
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = eventLoader->getFile()->getRecoJets().getParticles();
        for (const auto &p : particles)
        {
            particleList.addParticle(p);
        }
    }
    return particleList;
}

/*
std::vector<PileupSummaryInfo> InputModule::getPileupInfo() const
{
    return eventLoader->getPileupInfo();
}
*/

GenEventInfoProduct EventLoaderInputModule::getGenInfo() const
{
    return eventLoader->getFile()->getGenInfo();
}

double EventLoaderInputModule::getMET() const
{
    return eventLoader->getFile()->getMET();
} 

std::vector<bool> EventLoaderInputModule::getTriggerResults(std::string subProcess) const
{
    return eventLoader->getFile()->getTriggerResults(subProcess);
}

std::vector<std::string> EventLoaderInputModule::getTriggerNames(std::string subProcess) const
{
    return eventLoader->getFile()->getTriggerNames(subProcess);
}
