#include "CIAnalysis/CIStudies/interface/MiniAODEventLoader.hh"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

MiniAODEventLoader::MiniAODEventLoader(int outputEvery) : 
EventLoader(outputEvery)
{}

void MiniAODEventLoader::newFile(TFile* ifile)
{
    event = std::make_shared<fwlite::Event> (ifile);
    std::cerr << "Events: " << event->size() << std::endl;
    setNumOfEvents(getNumOfEvents() + event->size());
    event->toBegin();
}

void MiniAODEventLoader::nextEvent()
{
    auto& eventRef = *event;
    ++(eventRef);
    setEventCount(getEventCount() + 1);
}

/*
std::vector<PileupSummaryInfo> MiniAODEventLoader::getPileupInfo() const
{
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    event->getByLabel(std::string("slimmedAddPileupInfo"), pileup);
    return *pileup;
}
*/

GenEventInfoProduct MiniAODEventLoader::getGenInfo() const
{
    edm::Handle<GenEventInfoProduct> genInfo;
    event->getByLabel(std::string("generator"), genInfo);
    return *genInfo;
}

ParticleCollection MiniAODEventLoader::getGenSimParticles() const
{
    ParticleCollection genParticles;
    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event->getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
    for (const auto &p : *genParticlesHandle) 
    {
        genParticles.addParticle(Particle(&p));
    }
    return genParticles;
}

ParticleCollection MiniAODEventLoader::getRecoParticles() const
{
    ParticleCollection recoParticles;
    //This seems problematic
    

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

double MiniAODEventLoader::getMET() const
{
    edm::Handle<std::vector<pat::MET>> mets;
    event->getByLabel(edm::InputTag("slimmedMETs"), mets);
    for (const auto& p : *mets)
    {
        return p.corPt(pat::MET::METCorrectionLevel::Type1);
    }
    throw std::runtime_error("There are no MET objects found");

}

edm::Handle<edm::TriggerResults> MiniAODEventLoader::getTriggerResults(std::string subProcess) const
{
    edm::Handle<edm::TriggerResults> triggerResults;
    event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    return triggerResults;
}

edm::TriggerNames MiniAODEventLoader::getTriggerNames(std::string subProcess) const
{
    return event->triggerNames(*getTriggerResults(subProcess));
}

bool MiniAODEventLoader::isDone() const
{
    return event->atEnd();
}