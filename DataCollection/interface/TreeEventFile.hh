#ifndef TREEEVENTFILE_HH
#define TREEEVENTFILE_HH

#include "EventFile.hh"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>

class TTree;

class TreeEventFile : public EventFile
{
    public:
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
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
        TreeEventFile(TFile* ifile, const BranchNames& branchNames);
        virtual std::string getTreeName() = 0;
        //virtual BranchNames getTreeBranches() = 0;
        virtual void initialize();
    private:

        TTree* tree;
        TTreeReader treeReader;

        mutable TTreeReaderValue<Int_t> elec_size;
        mutable TTreeReaderArray<Float_t> elec_eta;
        mutable TTreeReaderArray<Float_t> elec_phi;
        mutable TTreeReaderArray<Float_t> elec_mass;
        mutable TTreeReaderArray<Int_t> elec_charge;
        mutable TTreeReaderArray<Float_t> elec_pt;
        mutable TTreeReaderArray<Float_t> elec_reliso;

        mutable TTreeReaderValue<Int_t> muon_size;
        mutable TTreeReaderArray<Float_t> muon_eta;
        mutable TTreeReaderArray<Float_t> muon_phi;
        mutable TTreeReaderArray<Float_t> muon_mass;
        mutable TTreeReaderArray<Int_t> muon_charge;
        mutable TTreeReaderArray<Float_t> muon_pt;
        mutable TTreeReaderArray<Float_t> muon_reliso;

        mutable TTreeReaderValue<Int_t> met_size;
        mutable TTreeReaderArray<Float_t> met_phi;
        mutable TTreeReaderArray<Float_t> met_pt;

        mutable TTreeReaderValue<Int_t> jet_size;
        mutable TTreeReaderArray<Float_t> jet_eta;
        mutable TTreeReaderArray<Float_t> jet_phi;
        mutable TTreeReaderArray<Float_t> jet_mass;
        mutable TTreeReaderArray<Float_t> jet_pt;
        //mutable TTreeReaderArray<Int_t> bJet;

        mutable TTreeReaderValue<Int_t> gen_size;
        mutable TTreeReaderArray<Int_t> gen_pid;
        mutable TTreeReaderArray<Int_t> gen_status;
        mutable TTreeReaderArray<Float_t> gen_eta;
        mutable TTreeReaderArray<Float_t> gen_phi;
        mutable TTreeReaderArray<Float_t> gen_mass;
        mutable TTreeReaderArray<Float_t> gen_pt;
        mutable TTreeReaderArray<Int_t> gen_d1;
        mutable TTreeReaderArray<Int_t> gen_d2;
        mutable TTreeReaderArray<Int_t> gen_m1;
        mutable TTreeReaderArray<Int_t> gen_m2;

        mutable TTreeReaderArray<unsigned> elec_idpass;
        mutable TTreeReaderArray<unsigned> muon_idpass;
};

#endif
