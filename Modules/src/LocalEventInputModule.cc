#include "CMSAnalysis/Modules/interface/LocalEventInputModule.hh"

#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

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
    auto particles = event->getParticles(level).getParticles();
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

int LocalEventInputModule::getNumPileUpInteractions() const
{
    throw std::runtime_error("GenSimEventFile has no implementation of getNumPileUpInteractions");
}

double LocalEventInputModule::getMET() const
{
    return event->getMET();
} 

std::vector<bool> LocalEventInputModule::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

std::vector<std::string> LocalEventInputModule::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

bool LocalEventInputModule::checkTrigger(std::string triggerName, std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

ParticleCollection<Particle> LocalEventInputModule::getSpecial(std::string key) const
{
    return event->getSpecial(key);
}