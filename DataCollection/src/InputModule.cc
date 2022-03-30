#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include <fstream>

InputModule::InputModule(const EventLoader *iEventLoader) : eventLoader(iEventLoader)
{
}

ParticleCollection InputModule::getLeptons(RecoLevel level) const
{
    ParticleCollection leptons;
    auto electrons = getParticles(level, Particle::Type::Electron).getParticles();
    auto muons = getParticles(level, Particle::Type::Muon).getParticles();
    for (const auto &p : electrons)
    {
        if ((!leptonSelector || leptonSelector->checkParticle(p))&& p.getPt()>=50&& p.isFinalState())
        {
            leptons.addParticle(p);
            //std::cout << std::setw(13) << p.getPt() << "| " << std::setw(13) << p.getEta() << "| " << std::setw(13) << p.getPhi() << "| " << std::setw(13) << p.energy() << "| " << std::setw(13) << p.getMass() << "\n";
        }
    }
    for (const auto &p : muons)
    {
        if ((!leptonSelector || leptonSelector->checkParticle(p))&& p.getPt()>=50 && p.isFinalState())
        {
            leptons.addParticle(p);
            //std::cout << std::setw(13) << p.getPt() << "| " << std::setw(13) << p.getEta() << "| " << std::setw(13) << p.getPhi() << "| " << std::setw(13) << p.energy() << "| " << std::setw(13) << p.getMass() << "\n";

        }
        //my_file <<"\n";
        //my_file <<counter;
        //my_file <<"\n";
        //my_file.close();
    }
    // if(leptons.getNumParticles()==4){
    //     std::ofstream my_file;
    //     my_file.open("EventDumpWWZ.txt", std::ios::app);
    //     for(const auto &p :leptons){
    //         my_file << std::setw(13)<< p.pdgId()<<"| " << std::setw(13) << p.getPt() << "| " << std::setw(13) << p.getEta() << "| " << std::setw(13) << p.getPhi() << "| " << std::setw(13) << p.energy() << "| " << std::setw(13) << p.getMass() << "\n";
    //     }
    //     my_file <<"\n";
    //     //my_file <<counter;
    //     //my_file <<"\n";
    //     my_file.close();
    // }
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
            if ((p.getType() == particleType || particleType == Particle::Type::None))
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
    //std::cout<<__FILE__<<" "<<__LINE__<<std::endl;
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
        auto particles = eventLoader->getFile()->getRecoJets().getParticles();
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
    return eventLoader->getFile()->getGenInfo();
}

double InputModule::getMET() const
{
    return eventLoader->getFile()->getMET();
}       
std::vector<bool> InputModule::getTriggerResults(std::string subProcess) const
{
    return eventLoader->getFile()->getTriggerResults(subProcess);
}
std::vector<std::string> InputModule::getTriggerNames(std::string subProcess) const
{
    return eventLoader->getFile()->getTriggerNames(subProcess);
}
