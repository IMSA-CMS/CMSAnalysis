#ifndef LEPTONJETDATASTRIPMODULE_H
#define LEPTONJETDATASTRIPMODULE_H

#include "AnalysisModule.hh"
#include "Rtypes.h"

class TFile;
class TTree;
class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

class LeptonJetDataStripModule : public AnalysisModule
{
public:
    virtual void initialize() override;

    LeptonJetDataStripModule(std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> recomod, std::shared_ptr<LeptonJetMatchingModule> matchmod);

    virtual void finalize() override;                                                     

    virtual std::string getName() override {return "LeptonJetDataStripModule";}
    
private:
    TFile *file;
    TTree *tree;
    TTree *tree2;
    Float_t leadingPt;
    Int_t jetIndex;
    Int_t nParticles; //? not jetIndex?
    Float_t phi;
    Float_t eta;
    Float_t mass;
    Float_t deltaR;
    Float_t sumPt;
    Float_t deltaPt;
    Int_t numMuons; //new one start working on this

    const std::shared_ptr<LeptonJetReconstructionModule> recomod;
    const std::shared_ptr<LeptonJetMatchingModule> matchmod;

protected:
    virtual bool process() override;
};

#endif