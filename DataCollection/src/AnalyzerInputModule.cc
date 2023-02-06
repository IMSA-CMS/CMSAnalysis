#include "CMSAnalysis/DataCollection/interface/AnalyzerInputModule.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"

AnalyzerInputModule::AnalyzerInputModule(const EventInterface** iEventInterface):
eventInterface(iEventInterface)
{
}

ParticleCollection<Lepton> AnalyzerInputModule::getLeptons(RecoLevel level) const
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

ParticleCollection<Particle> AnalyzerInputModule::getParticles(RecoLevel level, const ParticleType& particleType) const
{
    ParticleCollection<Particle> particleList;
    if (level == RecoLevel::GenSim)
    {
        auto particles = (*eventInterface)->getGenSimParticles().getParticles();
        for (const auto &p : particles)
        {
             if ((p.getType() == particleType || particleType == ParticleType::none())) //&& p.isFinalState())
            {
                particleList.addParticle(p);
            }
        }
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = (*eventInterface)->getRecoParticles().getParticles();
        

        for (const auto &p : particles)
        {
             if (p.getType() == particleType || particleType == ParticleType::none()) //&& p.isFinalState())
            {
                particleList.addParticle(p);
                //std::cout << "AnalyzerInputModule +1" ;
            }
        }
    }
    return particleList;
}

/* TODO: getJets */
ParticleCollection<Particle> AnalyzerInputModule::getJets(RecoLevel level) const
{
    ParticleCollection<Particle> particleList;
    if (level == RecoLevel::GenSim)
    {
        throw std::runtime_error("GenSim Jets not implemented");
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = (*eventInterface)->getRecoJets().getParticles();
        for (const auto &p : particles)
        {
            particleList.addParticle(p);
        }
    }
    return particleList;
}

int AnalyzerInputModule::getNumPileUpInteractions() const
{
    return (*eventInterface)->getNumPileUpInteractions();
}

double AnalyzerInputModule::getMET() const
{
    return (*eventInterface)->getMET();
} 

std::vector<bool> AnalyzerInputModule::getTriggerResults(std::string subProcess) const
{
    return (*eventInterface)->getTriggerResults(subProcess);
}

std::vector<std::string> AnalyzerInputModule::getTriggerNames(std::string subProcess) const
{
    return (*eventInterface)->getTriggerNames(subProcess);
}

bool AnalyzerInputModule::checkTrigger(std::string triggerName, std::string subProcess) const
{
    return (*eventInterface)->checkTrigger(triggerName, subProcess);
}
