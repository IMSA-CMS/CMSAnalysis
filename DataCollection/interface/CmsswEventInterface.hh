#ifndef CMSSWEVENTINTERFACE_HH
#define CMSSWEVENTINTERFACE_HH

#include "ParticleCollection.hh"
#include "EventInterface.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//class TFile;

class CmsswEventInterface : public EventInterface
{
    public:
        CmsswEventInterface(
            edm::EDGetTokenT<pat::ElectronCollection>& iElectronToken,
            edm::EDGetTokenT<pat::MuonCollection>& iMuonToken,
            edm::EDGetTokenT<pat::PhotonCollection>& iPhotonToken,
            edm::EDGetTokenT<pat::JetCollection>& iJetToken,
            edm::EDGetTokenT<pat::METCollection>& iMETToken);

        // maybe just ++event
        
        void setEvent(const edm::Event *newEvent){event = newEvent;}
       // void setTokens(const std::vector<edm::EDGetTokenT<edm::DetSetVector<pat::PATObject*>>> *newTokens){tokens = newTokens;}
        ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        ParticleCollection<Particle> getRecoParticles() const override;
        ParticleCollection<Particle> getRecoJets() const override;
        double getMET() const override;
        std::vector<bool> getTriggerResults(std::string subProcess) const override;
        std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        int getNumPileUpInteractions() const override;
        bool checkTrigger(std::string triggerName, std::string subProcess) const override;
        
    private:
        const edm::Event* event = nullptr;
        edm::EDGetTokenT<pat::ElectronCollection>& electronToken;
        edm::EDGetTokenT<pat::MuonCollection>& muonToken;
        edm::EDGetTokenT<pat::PhotonCollection>& photonToken;
        edm::EDGetTokenT<pat::JetCollection>& jetToken;
        edm::EDGetTokenT<pat::METCollection>& metToken;
};



#endif
