#ifndef NANOAODEVENTLOADER_HH
#define NANOAODEVENTLOADER_HH

#include "EventFile.hh"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>


class TTree;

class NanoAODEventFile : public EventFile
{
    public:
        NanoAODEventFile(TFile* ifile);
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        //virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual int getNumPileUpInteractions() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override;
    private:

        TTree* tree;
        TTreeReader treeReader;

        mutable TTreeReaderValue<UInt_t> elec_size; // nanoAOD wants UInt, Delphes wants Int (for all size variables)
        mutable TTreeReaderArray<Float_t> elec_eta;
        mutable TTreeReaderArray<Float_t> elec_phi;
        mutable TTreeReaderArray<Float_t> elec_mass;
        mutable TTreeReaderArray<Int_t> elec_charge;
        mutable TTreeReaderArray<Float_t> elec_pt;
        mutable TTreeReaderArray<Float_t> elec_reliso;
        mutable TTreeReaderArray<Float_t> elec_dxy;
        mutable TTreeReaderArray<Float_t> elec_dz;

        mutable TTreeReaderValue<UInt_t> muon_size;
        mutable TTreeReaderArray<Float_t> muon_eta;
        mutable TTreeReaderArray<Float_t> muon_phi;
        mutable TTreeReaderArray<Float_t> muon_mass;
        mutable TTreeReaderArray<Int_t> muon_charge;
        mutable TTreeReaderArray<Float_t> muon_pt;
        mutable TTreeReaderArray<Float_t> muon_reliso;
        mutable TTreeReaderArray<Float_t> muon_dxy;
        mutable TTreeReaderArray<Float_t> muon_dz;

        //mutable TTreeReaderValue<UInt_t> met_size; useless variable
        mutable TTreeReaderArray<Float_t> met_phi;
        mutable TTreeReaderArray<Float_t> met_pt;

        mutable TTreeReaderValue<UInt_t> jet_size;
        mutable TTreeReaderArray<Float_t> jet_eta;
        mutable TTreeReaderArray<Float_t> jet_phi;
        mutable TTreeReaderArray<Float_t> jet_mass;
        mutable TTreeReaderArray<Float_t> jet_pt;
        //mutable TTreeReaderArray<Int_t> bJet;

        mutable TTreeReaderValue<UInt_t> gen_size;
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
        mutable TTreeReaderArray<Float_t> gen_pileup;

        mutable TTreeReaderArray<Int_t> elec_idpass;
        mutable TTreeReaderArray<Bool_t> muon_looseid;
        mutable TTreeReaderArray<Bool_t> muon_mediumid;
        mutable TTreeReaderArray<Bool_t> muon_tightid;
        mutable std::unordered_map<std::string, TTreeReaderValue<Bool_t>> triggers;

        std::vector<GenSimParticle> genSimParticles;
};


#endif