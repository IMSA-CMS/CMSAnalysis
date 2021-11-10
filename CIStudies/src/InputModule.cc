#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "CIAnalysis/CIStudies/interface/EventLoader.hh"

InputModule::InputModule(std::shared_ptr<EventLoader> iEventLoader) : eventLoader(iEventLoader)
{}

ParticleCollection InputModule::getLeptons(RecoLevel level, double pTcut) const
{
    ParticleCollection leptons;
    auto electrons = getParticles(level, Particle::Type::Electron, pTcut).getParticles();
    auto muons = getParticles(level, Particle::Type::Muon, pTcut).getParticles();
    for (const auto& p : electrons)
    {
        leptons.addParticle(p);
    }
    for (const auto& p : muons)
    {
        leptons.addParticle(p);
    }
    return leptons;
}

ParticleCollection InputModule::getParticles(RecoLevel level, Particle::Type particleType, double pTcut) const
{
    ParticleCollection particleList;
    if (level == RecoLevel::GenSim)
    {
        auto particles = eventLoader->getGenSimParticles().getParticles();
        for (const auto &p : particles)
        {
	  if ((p.getType() == particleType || particleType == Particle::Type::None) && p.isFinalState() && p.getPt() >= pTcut) 
            {    
                particleList.addParticle(p);
            }
        }
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = eventLoader->getRecoParticles().getParticles();
        for (const auto &p : particles)
        {
	  if ((p.getType() == particleType || particleType == Particle::Type::None) && p.getPt() >= pTcut)
            {
                particleList.addParticle(p);
            }
        }
    }
    return particleList;
}

/* TODO: getJets */
ParticleCollection InputModule::getJets(RecoLevel level, double pTcut) const
{
    ParticleCollection particleList;
    if (level == RecoLevel::GenSim)
    {
        throw std::runtime_error("GenSim Jets not implemented");
    }
    else if (level == RecoLevel::Reco)
    {
        auto particles = eventLoader->getRecoJets().getParticles();
        for (const auto &p : particles)
        {
	  if (p.getPt() >= pTcut)
            {
                particleList.addParticle(p);
            }
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

GenEventInfoProduct InputModule::getGenInfo() const
{
    return eventLoader->getGenInfo();
}

double InputModule::getMET() const
{
    return eventLoader->getMET();
}
// edm::TriggerResults InputModule::getTriggerResults(std::string subProcess) const
// {
//     return eventLoader->getTriggerResults(subProcess);
// }
// edm::TriggerNames InputModule::getTriggerNames(std::string subProcess) const
// {
//     return eventLoader->getTriggerNames(subProcess);
// }
