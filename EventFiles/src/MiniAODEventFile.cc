#include "CMSAnalysis/EventFiles/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

MiniAODEventFile::MiniAODEventFile(TFile* ifile) : 
EventFile(ifile)
{
    event = std::make_shared<fwlite::Event> (getFile());
    setEventCount(1);
    event->toBegin();
}

void MiniAODEventFile::nextEvent()
{
    auto& eventRef = *event;
    ++(eventRef);
    setEventCount(getEventCount() + 1);
}

ParticleCollection<GenSimParticle> MiniAODEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event->getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
    
    for (const auto &p : *genParticlesHandle) 
    {
        genParticles.addParticle(GenSimParticle(&p));
    }
    return genParticles;
}

ParticleCollection<Particle> MiniAODEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;

        edm::Handle<std::vector<pat::Electron>> electrons;
        event->getByLabel(std::string("slimmedElectrons"), electrons);

        for (const auto& p : *electrons)
	    {       
	        recoParticles.addParticle(Particle(&p)); 
	    }

        edm::Handle<std::vector<pat::Muon>> muons;
        event->getByLabel(edm::InputTag("slimmedMuons"), muons);

        for (const auto& p : *muons)
	    {       
	        recoParticles.addParticle(Particle(&p));
        }

        edm::Handle<std::vector<pat::Photon>> photons;
        event->getByLabel(edm::InputTag("slimmedPhotons"), photons);

        for (const auto& p : *photons)
	    {       
	        recoParticles.addParticle(Particle(&p));
        }
        return recoParticles;
}

ParticleCollection<Particle> MiniAODEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;

        edm::Handle<std::vector<pat::Jet>> jets;
        event->getByLabel(edm::InputTag("slimmedJets"), jets);

        for (const auto& j : *jets)
	    {       
	        recoParticles.addParticle(Particle(&j));
        }
        return recoParticles;
}

int MiniAODEventFile::getNumPileUpInteractions() const {
    
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    event->getByLabel(std::string("slimmedAddPileupInfo"), pileup);

    return (*pileup)[0].getPU_NumInteractions();

}

double MiniAODEventFile::getMET() const
{
    edm::Handle<std::vector<pat::MET>> mets;
    event->getByLabel(edm::InputTag("slimmedMETs"), mets);
    for (const auto& p : *mets)
    {
        return p.corPt(pat::MET::METCorrectionLevel::Type1);
    }
    throw std::runtime_error("There are no MET objects found");
}

std::vector<bool> MiniAODEventFile::getTriggerResults(std::string subProcess) const
{
    edm::Handle<edm::TriggerResults> triggerResults;
    event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    std::vector<bool> v_results = {};
    for (unsigned int i = 0; i < triggerResults->size(); i++)
    {
        v_results.push_back(triggerResults->accept(i));
    }
    return v_results;
}

std::vector<std::string> MiniAODEventFile::getTriggerNames(std::string subProcess) const
{
    // cout statements
    // std::cout << "Is this part running?" << "\n";
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

bool MiniAODEventFile::checkTrigger(std::string triggerName, std::string subProcess) const
{
    // cout statements
    // std::cout << "Is this part running?" << "\n";
    auto names = getTriggerNames(subProcess);
    auto results = getTriggerResults(subProcess);

    auto it = find(names.begin(), names.end(), triggerName);
    if(it != names.end())
    {
        // cout statement (if it is false in here)
        // std::cout << "Is this part running?" << "\n";
        return results.at(it - names.begin());
    }
    else
    {
        // cout statement (if it is false in here)
        // std::cout << "Is this part running?" << "\n";
        return false;
    }
}

bool MiniAODEventFile::isDone() const
{
    return event->atEnd();
}
