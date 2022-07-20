#ifndef TREEEVENTFILE_HH
#define TREEEVENTFILE_HH

#include "EventFile.hh"

class TTree;

class TreeEventFile : public EventFile
{
    public:
        TreeEventFile(TFile* ifile);
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
    protected:
        struct BranchNames
        {
            std::string elecSize;
            std::string elecCharge;
            std::string elecEta;
            std::string elecPhi;
            std::string elecMass;
            std::string elecPT;
            std::string elecRelIso;

            std::string muonSize;
            std::string muonCharge;
            std::string muonEta;
            std::string muonPhi;
            std::string muonMass;
            std::string muonPT;
            std::string muonRelIso;

            std::string metSize;
            std::string metPhi;
            std::string metPT;

            std::string jetSize;
            std::string jetEta;
            std::string jetPhi;
            std::string jetMass;
            std::string jetPT;
            std::string bJet;

            std::string genSize;
            std::string genPid;
            std::string genStatus;
            std::string genEta;
            std::string genPhi;
            std::string genMass;
            std::string genPt;
            std::string gend1;
            std::string gend2;
            std::string genm1;
            std::string genm2;

            std::string elecIdpass;
            std::string muonIdpass;
        };
        
        virtual std::string getTreeName() = 0;
        virtual BranchNames getTreeBranches() = 0;
        virtual void initialize();
    private:
        // Next time: fill in names from struct - make a protected abstract function =0
        // that just fills in and returns this struct
        // advantage of struct is that you need to name the things to put in the struct
        // new function (or the same) that includes the tree name
        static const Int_t MAXMEC = 100;
        static const Int_t MAXMC = 10000;
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
        Int_t elec_idpass[MAXMEC];
        Float_t elec_reliso[MAXMEC];

        Int_t muon_size;
        Int_t muon_charge[MAXMEC];
        Float_t muon_eta[MAXMEC];
        Float_t muon_phi[MAXMEC];
        Float_t muon_mass[MAXMEC];
        Float_t muon_pt[MAXMEC];
        Int_t muon_idpass[MAXMEC];
        Float_t muon_reliso[MAXMEC];

        Int_t met_size;
        Float_t met_phi[MAXMEC];
        Float_t met_pt[MAXMEC];

        Int_t jet_size;
        Float_t jet_eta[MAXMEC];
        Float_t jet_phi[MAXMEC];
        Float_t jet_mass[MAXMEC];
        Float_t jet_pt[MAXMEC];
        Int_t bJet[MAXMEC];

        Int_t gen_size;
        Int_t gen_pid[MAXMC];
        Int_t gen_status[MAXMC];
        Float_t gen_eta[MAXMC];
        Float_t gen_phi[MAXMC];
        Float_t gen_mass[MAXMC];
        Float_t gen_pt[MAXMC];
        Int_t gen_d1[MAXMC];
        Int_t gen_d2[MAXMC];
        Int_t gen_m1[MAXMC];
        Int_t gen_m2[MAXMC];
        
};

#endif
