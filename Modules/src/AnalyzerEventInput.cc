#include "CMSAnalysis/Modules/interface/AnalyzerEventInput.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"

AnalyzerEventInput::AnalyzerEventInput(const EventInterface **iEventInterface) : eventInterface(iEventInterface)
{
}

ParticleCollection<Lepton> AnalyzerEventInput::getLeptons(RecoLevel level) const
{
    ParticleCollection<Lepton> leptons;
    auto electrons = getParticles(level, ParticleType::electron()).getParticles();
    auto muons = getParticles(level, ParticleType::muon()).getParticles();
    //std::cout << "Electrons: " << electrons.size() << "\nMuons: " << muons.size() << "\n";
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

ParticleCollection<Particle> AnalyzerEventInput::getParticles(RecoLevel level, const ParticleType& particleType) const
{
    ParticleCollection<Particle> particleList;
    if (level == RecoLevel::GenSim)
    {
        auto particles = (*eventInterface)->getGenSimParticles().getParticles();
        //std::cout << particles.size() << "\n";
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
                //std::cout << "AnalyzerEventInput +1" ;
            }
        }
    }
    return particleList;
}

/* TODO: getJets */
ParticleCollection<Particle> AnalyzerEventInput::getJets(RecoLevel level) const
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

int AnalyzerEventInput::getNumPileUpInteractions() const
{
    return (*eventInterface)->getNumPileUpInteractions();
}

double AnalyzerEventInput::getMET() const
{
    return (*eventInterface)->getMET();
}

unsigned long long AnalyzerEventInput::getEventIDNum() const
{
    return (*eventInterface)->getEventIDNum();
} 

long AnalyzerEventInput::getRunNum() const
{
    return (*eventInterface)->getRunNum();
}

std::vector<bool> AnalyzerEventInput::getTriggerResults(std::string subProcess) const
{
    return (*eventInterface)->getTriggerResults(subProcess);
}

std::vector<std::string> AnalyzerEventInput::getTriggerNames(std::string subProcess) const
{
    
    return (*eventInterface)->getTriggerNames(subProcess);
}
//hmm
bool AnalyzerEventInput::checkTrigger(std::string triggerName, std::string subProcess) const
{
    return (*eventInterface)->checkTrigger(triggerName, subProcess);
}

const std::shared_ptr<FileParams> AnalyzerEventInput::getFileParams() const
{
    return (*eventInterface)->getFileParams();
}
std::vector<double> AnalyzerEventInput::getPDFWeights() const
{
    return (*eventInterface)->getPDFWeights();
}
