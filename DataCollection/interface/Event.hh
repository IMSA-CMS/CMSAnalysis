#ifndef EVENT_HH
#define EVENT_HH
#include <vector>

#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Photon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

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
        ParticleCollection<Particle> getSpecial(std::string key) const;

        ParticleCollection<Particle> getParticles() const;

        void addElectron(Electron electron);
        void addMuon(Muon muon);
        void addPhoton(Photon photon);
        void addJet(Jet jet);
        void addSpecialObject(std::string key, Particle obj); 
        void setMET(double newMET) {MET = newMET;}
        
        bool containsParticles() const;

        // Clears all vectors of particles within, for reuse of an Event object
        void clear();
    private:
    //make particlecollections
        ParticleCollection<Electron> electrons;
        ParticleCollection<Muon> muons;
        ParticleCollection<Photon> photons;
        ParticleCollection<Jet> jets;
        double MET;
        std::map<std::string, ParticleCollection<Particle>> specialObjects;
};
#endif