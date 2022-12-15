#ifndef DARKPHOTONGENSIMSELECTOR_HH
#define DARKPHOTONGENSIMSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

//Selects relevant particles for GenSim DarkPhoton Events (and feeds them to an EventModule)
//Implement in a DataCollectionPlan with a shared pointer then add to an EventModule with ->addSelector() function

class InputModule;

class DarkPhotonGenSimSelector : public Selector
{
    public:
        //Selects Dark Photons, their Lepton Daughters, Neutralinos, Leptons in their jets and Z Bosons
        void selectParticles(const InputModule* input, Event& event) override;
    private:
        //Recursively runs through a Jet to check for Leptons
        std::vector<Particle> checkJet(GenSimParticle part) const;
};

#endif