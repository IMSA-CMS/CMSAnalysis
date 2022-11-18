#ifndef STRIPPEDEVENTLOADER_HH
#define STRIPPEDEVENTLOADER_HH

#include "EventFile.hh"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TFile.h"

class TTree;

// An EventFile that works with stripped files generated from FileStripModule
class StrippedEventFile : public EventFile
{
  public:
    StrippedEventFile(TFile* ifile);
  protected:
    virtual void nextEvent() override;
    virtual bool isDone() const override;
    virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
    virtual ParticleCollection<Particle> getRecoParticles() const override;
    virtual ParticleCollection<Particle> getRecoJets() const override;
    virtual int getNumPileUpInteractions() const override {throw std::runtime_error("not implemented yet");}
    virtual double getMET() const override;
    virtual std::vector<bool> getTriggerResults(std::string subProcess) const override {throw std::runtime_error("not implemented yet");}
    virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override {throw std::runtime_error("not implemented yet");}
    virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override {throw std::runtime_error("not implemented yet");}
  private:
    TTree* tree;
    TTreeReader treeReader;

    mutable TTreeReaderValue<UInt_t> elecSize;
    mutable TTreeReaderArray<float> elecEta;
    mutable TTreeReaderArray<float> elecPhi;
    mutable TTreeReaderArray<float> elecMass;
    mutable TTreeReaderArray<int> elecCharge;
    mutable TTreeReaderArray<float> elecPt;
    mutable TTreeReaderArray<float> elecReliso;

    mutable TTreeReaderValue<UInt_t> muonSize;
    mutable TTreeReaderArray<float> muonPhi;
    mutable TTreeReaderArray<float> muonEta;
    mutable TTreeReaderArray<float> muonMass;
    mutable TTreeReaderArray<int> muonCharge;
    mutable TTreeReaderArray<float> muonPt;
    mutable TTreeReaderArray<float> muonReliso;

    mutable TTreeReaderArray<float> metPhi;
    mutable TTreeReaderArray<float> metPt;

    mutable TTreeReaderValue<UInt_t> jetSize;
    mutable TTreeReaderArray<float> jetEta;
    mutable TTreeReaderArray<float> jetPhi;
    mutable TTreeReaderArray<float> jetMass;
    mutable TTreeReaderArray<float> jetPt;

    mutable TTreeReaderValue<UInt_t> genSize;
    mutable TTreeReaderArray<int> genPID;
    mutable TTreeReaderArray<int> genStatus;
    mutable TTreeReaderArray<float> genEta;
    mutable TTreeReaderArray<float> genPhi;
    mutable TTreeReaderArray<float> genMass;
    mutable TTreeReaderArray<float> genPt;
    mutable TTreeReaderArray<int> genD1;
    mutable TTreeReaderArray<int> genD2;
    mutable TTreeReaderArray<int> genM1;
    mutable TTreeReaderArray<int> genM2;

    mutable TTreeReaderArray<int> elecIDPass;
    mutable TTreeReaderArray<int> muonIDPass;
};

#endif