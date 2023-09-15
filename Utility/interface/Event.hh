#ifndef EVENT_HH
#define EVENT_HH
#include <vector>

#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Photon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

// Stores an event by containing ParticleCollections of different types of Particles, as well as MET.
// Used in EventModule, where cuts are applied to it.
class Event
{
    public:
        Event();
        const ParticleCollection<Electron>& getElectrons() const {return electrons;};
        const ParticleCollection<Muon>& getMuons() const {return muons;};
        const ParticleCollection<Photon>& getPhotons() const {return photons;};
        const ParticleCollection<Jet>& getJets() const {return jets;};
        const double& getMET() const {return MET;};
        const std::map<std::string, ParticleCollection<Particle>>& getSpecials() const {return specialObjects;};

        // gets a special object by key
        ParticleCollection<Particle> getSpecial(std::string key) const;
        // get all particles
        ParticleCollection<Particle> getParticles(EventInput::RecoLevel level = EventInput::RecoLevel::Reco) const;

        void addElectron(Electron electron);
        void addMuon(Muon muon);
        void addPhoton(Photon photon);
        void addJet(Jet jet);
        void addGenSimParticle(GenSimParticle particle);
        void addSpecialObject(std::string key, Particle obj); 
        void setMET(double newMET) {MET = newMET;}
        
        // returns true if the event contains any particles
        bool containsParticles() const;

        // Clears all ParticleCollections/specialObjects, for reuse of the same Event object
        void clear();
    private:
        //make particlecollections
        ParticleCollection<Electron> electrons;
        ParticleCollection<Muon> muons;
        ParticleCollection<Photon> photons;
        ParticleCollection<Jet> jets;
        ParticleCollection<GenSimParticle> genSimParticles;
        double MET;
        std::map<std::string, ParticleCollection<Particle>> specialObjects;
};
#endif