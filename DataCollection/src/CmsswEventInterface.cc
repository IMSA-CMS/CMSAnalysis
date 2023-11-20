#include "CMSAnalysis/DataCollection/interface/CmsswEventInterface.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"

/*
std::vector<PileupSummaryInfo> MiniAODEventLoader::getPileupInfo() const
{
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    event->getByLabel(std::string("slimmedAddPileupInfo"), pileup);
    return *pileup;
}
*/
CmsswEventInterface::CmsswEventInterface(
    edm::EDGetTokenT<pat::ElectronCollection> &iElectronToken,
    edm::EDGetTokenT<pat::MuonCollection> &iMuonToken,
    edm::EDGetTokenT<pat::PhotonCollection> &iPhotonToken,
    edm::EDGetTokenT<pat::JetCollection> &iJetToken,
    edm::EDGetTokenT<pat::METCollection> &iMETToken,
    edm::EDGetTokenT<pat::TriggerEvent> &iTriggerToken,
    edm::EDGetTokenT<reco::GenParticleCollection> &iGenSimToken) : electronToken(iElectronToken),
                                                                   muonToken(iMuonToken),
                                                                   photonToken(iPhotonToken),
                                                                   jetToken(iJetToken),
                                                                   metToken(iMETToken),
                                                                   triggerToken(iTriggerToken),
                                                                   genSimToken(iGenSimToken)
{
}

ParticleCollection<GenSimParticle> CmsswEventInterface::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;

    // edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;

    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event->getByToken(genSimToken, genParticlesHandle);

    // event->getByLabel(std::string("prunedGenParticles"), genParticlesHandle);

    for (const auto &p : *genParticlesHandle)
    {
        genParticles.addParticle(GenSimParticle(&p));
    }
    return genParticles;
}

ParticleCollection<Particle> CmsswEventInterface::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    // This seems problematic

    edm::Handle<std::vector<pat::Electron>> electrons;
    event->getByToken(electronToken, electrons);

    for (const auto &p : *electrons)
    {
        recoParticles.addParticle(Particle(&p));
    }

    edm::Handle<std::vector<pat::Muon>> muons;
    event->getByToken(muonToken, muons);

    for (const auto &p : *muons)
    {
        recoParticles.addParticle(Particle(&p));
    }

    edm::Handle<std::vector<pat::Photon>> photons;
    event->getByToken(photonToken, photons);

    for (const auto &p : *photons)
    {
        recoParticles.addParticle(Particle(&p));
    }
    return recoParticles;
}

ParticleCollection<Particle> CmsswEventInterface::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    edm::Handle<std::vector<pat::Jet>> jets;
    event->getByToken(jetToken, jets);
    for (const auto &j : *jets)
    {
        recoParticles.addParticle(Particle(&j));
    }
    return recoParticles;
}

double CmsswEventInterface::getMET() const
{
    edm::Handle<std::vector<pat::MET>> mets;

    event->getByToken(metToken, mets);

    for (const auto &p : *mets)
    {
        return p.corPt(pat::MET::METCorrectionLevel::Type1);
    }
    throw std::runtime_error("There are no MET objects found");
}

std::vector<bool> CmsswEventInterface::getTriggerResults(std::string subProcess) const
{
    edm::Handle<std::vector<pat::TriggerEvent>> triggerResults;
    event->getByToken(triggerToken, triggerResults);
    // event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    std::vector<bool> v_results = {};
    for (unsigned int i = 0; i < triggerResults->size(); i++)
    {
        v_results.push_back(triggerResults->at(i).wasAccept());
    }
    return v_results;
}

std::vector<std::string> CmsswEventInterface::getTriggerNames(std::string subProcess) const
{

    edm::Handle<std::vector<pat::TriggerEvent>> triggerResults;
    event->getByToken(triggerToken, triggerResults);
    // event->getByLabel(edm::InputTag("TriggerResults", "", subProcess), triggerResults);
    // const edm::TriggerNames names = event->triggerNames(*triggerResults);
    std::vector<std::string> v_names = {};
    for (unsigned int i = 0; i < triggerResults->size(); i++)
    {
        v_names.push_back(triggerResults->at(i).nameHltTable());
    }
    return v_names;
}

int CmsswEventInterface::getNumPileUpInteractions() const
{
    return 1;
    // return static_cast<int>(gen_pileup[0]);
}
bool CmsswEventInterface::checkTrigger(std::string triggerName, std::string subProcess) const
{
    auto names = getTriggerNames(subProcess);
    auto results = getTriggerResults(subProcess);

    auto it = find(names.begin(), names.end(), triggerName);
    if (it != names.end())
    {
        return results.at(it - names.begin());
    }
    else
    {
        return false;
    }
    // return *(triggers.find(triggerName)->second);
}