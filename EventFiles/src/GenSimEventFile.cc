#include "CMSAnalysis/EventFiles/interface/GenSimEventFile.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

GenSimEventFile::GenSimEventFile(TFile* ifile) : 
EventFile(ifile)
{
    event = std::make_shared<fwlite::Event> (getFile());
    setEventCount(1);
    event->toBegin();
}

void GenSimEventFile::nextEvent()
{
    auto& eventRef = *event;
    ++(eventRef);
    setEventCount(getEventCount() + 1);
}

ParticleCollection<GenSimParticle> GenSimEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event->getByLabel(std::string("genParticles"), genParticlesHandle);
    for (const auto &p : *genParticlesHandle) 
    {
        genParticles.addParticle(Particle(&p));
    }
    return genParticles;
}

ParticleCollection<Particle> GenSimEventFile::getRecoParticles() const
{
    return ParticleCollection<Particle>();
}

ParticleCollection<Particle> GenSimEventFile::getRecoJets() const
{
    return ParticleCollection<Particle>();
}

int GenSimEventFile::getNumPileUpInteractions() const
{
    return -1;
}

double GenSimEventFile::getMET() const
{
    edm::Handle<std::vector<reco::GenMET>> mets;
    event->getByLabel(edm::InputTag("genMetTrue"), mets);
    for (const auto& p : *mets)
    {
        return std::sqrt(p.p4().Perp2()); 
    }
    throw std::runtime_error("There are no MET objects found");
}

std::vector<bool> GenSimEventFile::getTriggerResults(std::string subProcess) const
{
    edm::Handle<edm::TriggerResults> triggerResults;
    event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    std::vector<bool> v_results = {};
    if (triggerResults) 
    {
        for (unsigned int i = 0; i < triggerResults->size(); i++)
        {
        v_results.push_back(triggerResults->accept(i));
        }
    }
    return v_results;
}

std::vector<std::string> GenSimEventFile::getTriggerNames(std::string subProcess) const
{
    edm::Handle<edm::TriggerResults> triggerResults;
    event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    const edm::TriggerNames names = event->triggerNames(*triggerResults);
    std::vector<std::string> v_names = {};
    for (unsigned int i = 0; i < names.size(); i++)
    {
        v_names.push_back(names.triggerName(i));
    }
    return v_names;
}

bool GenSimEventFile::checkTrigger(std::string triggerName, std::string subProcess) const
{
    auto names = getTriggerNames(subProcess);
    auto results = getTriggerResults(subProcess);

    auto it = find(names.begin(), names.end(), triggerName);
    if(it != names.end())
    {
        return results.at(it - names.begin());
    }
    else
    {
        return false;
    }
}

bool GenSimEventFile::isDone() const
{
    return event->atEnd();
}

std::vector<double> GenSimEventFile::getPDFWeights() const
{
    throw "Function not implemented";
}