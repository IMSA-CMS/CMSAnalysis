#include "CIAnalysis/CIStudies/interface/MiniAODEventFile.hh"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
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
    std::cerr << "Events: " << event->size() << std::endl;
    setNumOfEvents(getNumOfEvents() + event->size());
    event->toBegin();
}

void MiniAODEventFile::nextEvent()
{
    auto& eventRef = *event;
    ++(eventRef);
    setEventCount(getEventCount() + 1);
    // std::cout << "next Event \n";
}

/*
std::vector<PileupSummaryInfo> MiniAODEventLoader::getPileupInfo() const
{
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    event->getByLabel(std::string("slimmedAddPileupInfo"), pileup);
    return *pileup;
}
*/

GenEventInfoProduct MiniAODEventFile::getGenInfo() const
{
    edm::Handle<GenEventInfoProduct> genInfo;
    event->getByLabel(std::string("generator"), genInfo);
    return *genInfo;
}

ParticleCollection MiniAODEventFile::getGenSimParticles() const
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

ParticleCollection MiniAODEventFile::getRecoParticles() const
{
    // std::cout << "get reco particles \n";
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
        // std::cout << "made it through mini aod reco particles \n";
        return recoParticles;
}

ParticleCollection MiniAODEventFile::getRecoJets() const
{
    ParticleCollection recoParticles;

        edm::Handle<std::vector<pat::Jet>> jets;
        event->getByLabel(edm::InputTag("slimmedJets"), jets);

        for (const auto& j : *jets)
	    {       
	        recoParticles.addParticle(Particle(&j));
        }
        return recoParticles;
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

// edm::TriggerResults MiniAODEventLoader::getTriggerResults(std::string subProcess) const
// {
//     edm::Handle<edm::TriggerResults> triggerResults;
//     event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
//     return triggerResults;
// }

// edm::TriggerNames MiniAODEventLoader::getTriggerNames(std::string subProcess) const
// {
//     return event->triggerNames(*getTriggerResults(subProcess));
// }

bool MiniAODEventFile::isDone() const
{
    return event->atEnd();
}
