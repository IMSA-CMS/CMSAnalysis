#ifndef EVENT_HH
#define EVENT_HH
#include <vector>

#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Tau.hh"
#include "CMSAnalysis/Utility/interface/Photon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

// Stores an event by containing ParticleCollections of different types of Particles, as well as MET.
// Used in EventModule, where cuts are applied to it.
class Event
{
    public:
        Event(const EventInput* input);
        const ParticleCollection<Electron>& getElectrons() const {return electrons;};
        const ParticleCollection<Muon>& getMuons() const {return muons;};
        const ParticleCollection<Tau>& getTaus() const {return taus;};
        const ParticleCollection<Photon>& getPhotons() const {return photons;};
        const ParticleCollection<Jet>& getJets() const {return jets;};
        const reco::Candidate::LorentzVector& getMET() const {return MET;};
        const std::map<std::string, ParticleCollection<Particle>>& getSpecials() const {return specialObjects;};

        // gets a special object by key
        ParticleCollection<Particle> getSpecial(std::string key) const;
        // get all particles
        ParticleCollection<Particle> getParticles(EventInput::RecoLevel level = EventInput::RecoLevel::Reco, bool includeSpecials = true) const;
        // get all leptons
        ParticleCollection<Particle> getLeptons(EventInput::RecoLevel level = EventInput::RecoLevel::Reco) const;

        void addElectron(Electron electron);
        void addMuon(Muon muon);
        void addTau(Tau tau);
        void addPhoton(Photon photon);
        void addJet(Jet jet);
        void addGenSimParticle(GenSimParticle particle);
        void addSpecialObject(std::string key, Particle obj); 
        void setMET(reco::Candidate::LorentzVector newMET) {MET = newMET;}

        // returns true if the event contains any particles
        bool containsParticles() const;

        // Clears all ParticleCollections/specialObjects, for reuse of the same Event object
        void clear();

        const std::shared_ptr<FileParams> getFileParams() const {return input->getFileParams();}
        int getNumPileUpInteractions() const {return input->getNumPileUpInteractions();}
        unsigned long long getEventIDNum() const {return input->getEventIDNum();}
        long getRunNum() const {return input->getRunNum();}
        int getLumiBlock() const {return input->getLumiBlock();}
        void setInput(const EventInput* input1) {input = input1;}
    private:
        //make particlecollections
        ParticleCollection<Electron> electrons;
        ParticleCollection<Muon> muons;
        ParticleCollection<Tau> taus;
        ParticleCollection<Photon> photons;
        ParticleCollection<Jet> jets;
        ParticleCollection<GenSimParticle> genSimParticles;
        reco::Candidate::LorentzVector MET;
        std::map<std::string, ParticleCollection<Particle>> specialObjects;
        const EventInput* input;
};
#endif