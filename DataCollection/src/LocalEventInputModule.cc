#include "CMSAnalysis/DataCollection/interface/LocalEventInputModule.hh"

#include "CMSAnalysis/DataCollection/interface/Module.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"

LocalEventInputModule::LocalEventInputModule(const Event* event1)
{
    event = event1;
}

ParticleCollection<Lepton> LocalEventInputModule::getLeptons(RecoLevel level) const
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

ParticleCollection<Particle> LocalEventInputModule::getParticles(RecoLevel level, Particle::Type particleType) const
{
    ParticleCollection<Particle> particleList;
    auto particles = event->getParticles().getParticles();
    for (const auto &p : particles)
    {
        if ((p.getType() == particleType || particleType == Particle::Type::None))
        {
            particleList.addParticle(p);
        }
    }
    return particleList;
}

/* TODO: getJets */
ParticleCollection<Particle> LocalEventInputModule::getJets(RecoLevel level) const
{
    return event->getJets();
}

/*
std::vector<PileupSummaryInfo> InputModule::getPileupInfo() const
{
    return eventLoader->getPileupInfo();
}
*/

double LocalEventInputModule::getMET() const
{
    return event->getMET();
} 

std::vector<bool> LocalEventInputModule::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
    return std::vector<bool>(0);
}

std::vector<std::string> LocalEventInputModule::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
    return std::vector<std::string>(0);
}
