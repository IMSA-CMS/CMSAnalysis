#include "CMSAnalysis/DataCollection/interface/RootEventInterface.hh"
#include "CMSAnalysis/EventFiles/interface/EventFile.hh"

ParticleCollection<GenSimParticle> RootEventInterface::getGenSimParticles() const
{
    return eventFile->getGenSimParticles();
}
ParticleCollection<Particle> RootEventInterface::getRecoParticles() const
{
    return eventFile->getRecoParticles();
}
ParticleCollection<Particle> RootEventInterface::getRecoJets() const
{
    return eventFile->getRecoJets();
}
double RootEventInterface::getMET() const
{
    return eventFile->getMET();
}
unsigned long long RootEventInterface::getEventIDNum() const
{
    return eventFile->getEventIDNum(); 
}
long RootEventInterface::getRunNum() const
{
    return eventFile->getRunNum(); 
}
std::vector<bool> RootEventInterface::getTriggerResults(std::string subProcess) const
{
    return eventFile->getTriggerResults(subProcess);
}
std::vector<std::string> RootEventInterface::getTriggerNames(std::string subProcess) const
{
    return eventFile->getTriggerNames(subProcess);
}
int RootEventInterface::getNumPileUpInteractions() const
{
   return eventFile->getNumPileUpInteractions();
}
//hmm
bool RootEventInterface::checkTrigger(std::string triggerName, std::string subProcess) const
{
    return eventFile->checkTrigger(triggerName, subProcess);
}
const std::shared_ptr<FileParams> RootEventInterface::getFileParams() const
{
    return eventFile->getFileParams();
}
std::vector<double> RootEventInterface::getPDFWeights() const
{
    return eventFile->getPDFWeights();
}