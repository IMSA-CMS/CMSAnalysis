#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "CIAnalysis/CIStudies/interface/EventLoader.hh"

InputModule::InputModule(const EventLoader* iEventLoader) : eventLoader(iEventLoader)
{}

ParticleCollection InputModule::getLeptons(RecoLevel level) const
{
    ParticleCollection leptons;
    auto electrons = getParticles(level, Particle::Type::Electron).getParticles();
    auto muons = getParticles(level, Particle::Type::Muon).getParticles();
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

ParticleCollection InputModule::getParticles(RecoLevel level, Particle::Type particleType) const
{
    
    // std::cerr << "Input Module Test" << "\n";
    // std::cerr << eventLoader << "\n";
    // std::cerr << "Input Module Test 2" << "\n";
    ParticleCollection particleList;
    if (level == RecoLevel::GenSim)
    {  
        auto particles = eventLoader->getGenSimParticles().getParticles();
        // std::cout << "Event Loader One \n";
        for (const auto &p : particles) 
        {   
            // std::cout << "Four loop One \n";
            if ((p.getType() == particleType || particleType == Particle::Type::None) && p.isFinalState()) 
            {   
                //  std::cout << "If One \n";
                particleList.addParticle(p);
            }
        }
    }
    else if (level == RecoLevel::Reco)
    {
        // std::cout << "Reco (next eventLoader->getrecoParticle())\n";
        auto particles = eventLoader->getRecoParticles().getParticles();
        // std::cout << "Event Loader Two \n";
        for (const auto &p : particles)
        {
            // std::cout << "Four loop Two \n";
            if (p.getType() == particleType || particleType == Particle::Type::None)
            {
                // std::cout << "If Two \n";
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