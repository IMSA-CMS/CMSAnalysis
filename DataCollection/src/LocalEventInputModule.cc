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
    auto electrons = getParticles(level, ParticleType::electron()).getParticles();
    auto muons = getParticles(level, ParticleType::muon()).getParticles();
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

ParticleCollection<Particle> LocalEventInputModule::getParticles(RecoLevel level, const ParticleType& particleType) const
{
    ParticleCollection<Particle> particleList;
    auto particles = event->getParticles().getParticles();
    for (const auto &p : particles)
    {
        if (p.getType() == particleType || particleType == ParticleType::none())
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

ParticleCollection<Particle> LocalEventInputModule::getSpecial(std::string key) const
{
    return event->getSpecial(key);
}