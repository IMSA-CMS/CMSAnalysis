#include "CMSAnalysis/DataCollection/interface/CmsswEventInterface.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

CmsswEventInterface::CmsswEventInterface() : 
EventInterface()
{
}

/*
std::vector<PileupSummaryInfo> MiniAODEventLoader::getPileupInfo() const
{
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    event->getByLabel(std::string("slimmedAddPileupInfo"), pileup);
    return *pileup;
}
*/

ParticleCollection<GenSimParticle> CmsswEventInterface::getGenSimParticles() const
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

ParticleCollection<Particle> CmsswEventInterface::getRecoParticles() const
{
    // std::cout << "get reco particles \n";
    ParticleCollection<Particle> recoParticles;
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

ParticleCollection<Particle> CmsswEventInterface::getRecoJets() const
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

double CmsswEventInterface::getMET() const
{
    edm::Handle<std::vector<pat::MET>> mets;
    event->getByLabel(edm::InputTag("slimmedMETs"), mets);
    for (const auto& p : *mets)
    {
        return p.corPt(pat::MET::METCorrectionLevel::Type1);
    }
    throw std::runtime_error("There are no MET objects found");
}

std::vector<bool> CmsswEventInterface::getTriggerResults(std::string subProcess) const
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

std::vector<std::string> CmsswEventInterface::getTriggerNames(std::string subProcess) const
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
