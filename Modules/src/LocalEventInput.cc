#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"

#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

LocalEventInput::LocalEventInput(const Event* event1)
{
    event = event1;
} 

ParticleCollection<Particle> LocalEventInput::getParticles(RecoLevel level, const ParticleType& particleType, bool includeSpecials) const
{
    ParticleCollection<Particle> particleList;
    auto particles = event->getParticles(level, includeSpecials).getParticles();
    for (const auto &p : particles)
    {
        if (p.getType() == particleType || particleType == ParticleType::none())
        {
            particleList.addParticle(p);
        }
    }
    return particleList;
}

ParticleCollection<Particle> LocalEventInput::getJets(RecoLevel level) const
{
    return event->getJets();
}

int LocalEventInput::getNumPileUpInteractions() const
{
    return event->getNumPileUpInteractions();
}

reco::Candidate::LorentzVector LocalEventInput::getMET() const
{
    return event->getMET();
}

unsigned long long LocalEventInput::getEventIDNum() const
{
    return event->getEventIDNum();
} 

long LocalEventInput::getRunNum() const
{
    return event->getRunNum(); 
} 

int LocalEventInput::getLumiBlock() const
{
    return event->getLumiBlock();
}

std::vector<bool> LocalEventInput::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

std::vector<std::string> LocalEventInput::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

bool LocalEventInput::checkTrigger(std::string triggerName, std::string subProcess) const
{
    throw std::runtime_error("calling getTrigger___() on a local event doesn't make sense");
}

ParticleCollection<Particle> LocalEventInput::getSpecial(std::string key) const
{
    return event->getSpecial(key);
}

const std::shared_ptr<FileParams> LocalEventInput::getFileParams() const 
{
    return event->getFileParams();
}
std::vector<double> LocalEventInput::getPDFWeights() const
{
    throw "Function not implemented";
}