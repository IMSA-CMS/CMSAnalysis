#include "CMSAnalysis/Modules/interface/LocalEventEventInput.hh"

#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

LocalEventEventInput::LocalEventEventInput(const Event* event1)
{
    event = event1;
}

ParticleCollection<Lepton> LocalEventEventInput::getLeptons(RecoLevel level) const
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

ParticleCollection<Particle> LocalEventEventInput::getParticles(RecoLevel level, const ParticleType& particleType) const
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
ParticleCollection<Particle> LocalEventEventInput::getJets(RecoLevel level) const
{
    return event->getJets();
}

int LocalEventEventInput::getNumPileUpInteractions() const
{
    throw std::runtime_error("GenSimEventFile has no implementation of getNumPileUpInteractions");
}

double LocalEventEventInput::getMET() const
{
    return event->getMET();
} 

std::vector<bool> LocalEventEventInput::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

std::vector<std::string> LocalEventEventInput::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

bool LocalEventEventInput::checkTrigger(std::string triggerName, std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

ParticleCollection<Particle> LocalEventEventInput::getSpecial(std::string key) const
{
    return event->getSpecial(key);
}