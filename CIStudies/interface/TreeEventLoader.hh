#ifndef TREEEVENTLOADER_HH
#define TREEEVENTLOADER_HH

#include "EventLoader.hh"

class TTree;

class TreeEventLoader : public EventLoader
{
    public:
        using EventLoader::EventLoader;
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection getGenSimParticles() const override;
        virtual ParticleCollection getRecoParticles() const override;
        virtual ParticleCollection getRecoJets() const override;
        virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
    protected:
        virtual void newFile(TFile* ifile) override;
        struct BranchNames
        {
            std::string elecSize;
            std::string elecCharge;
            std::string elecEta;
            std::string elecPhi;
            std::string elecMass;
            std::string elecPT;

            std::string muonSize;
            std::string muonCharge;
            std::string muonEta;
            std::string muonPhi;
            std::string muonMass;
            std::string muonPT;

            std::string metSize;
            std::string metPhi;
            std::string metPT;

            std::string jetSize;
            std::string jetEta;
            std::string jetPhi;
            std::string jetMass;
            std::string jetPT;
        };
        
        virtual std::string getTreeName() = 0;
        virtual BranchNames getTreeBranches() = 0;
    private:
        // Next time: fill in names from struct - make a protected abstract function =0
        // that just fills in and returns this struct
        // advantage of struct is that you need to name the things to put in the struct
        // new function (or the same) that includes the tree name
        static const Int_t MAXMEC = 100;
        TTree* tree;
        
        // Not sure if this is correct
        Int_t counter;

        // Annoying variables, may God have mercy on my soul
        Int_t elec_size;
        Int_t elec_charge[MAXMEC];
        Float_t elec_eta[MAXMEC];
        Float_t elec_phi[MAXMEC];
        Float_t elec_mass[MAXMEC];
        Float_t elec_pt[MAXMEC];

        Int_t muon_size;
        Int_t muon_charge[MAXMEC];
        Float_t muon_eta[MAXMEC];
        Float_t muon_phi[MAXMEC];
        Float_t muon_mass[MAXMEC];
        Float_t muon_pt[MAXMEC];

        Int_t met_size;
        Float_t met_phi[MAXMEC];
        Float_t met_pt[MAXMEC];

        Int_t jet_size;
        Float_t jet_eta[MAXMEC];
        Float_t jet_phi[MAXMEC];
        Float_t jet_mass[MAXMEC];
        Float_t jet_pt[MAXMEC];
};



#endif
