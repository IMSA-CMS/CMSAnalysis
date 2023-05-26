#ifndef DATASTRIPMODULE_H
#define DATASTRIPMODULE_H

#include "AnalysisModule.hh"
#include "Rtypes.h"

class TFile;
class TTree;
class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

class DataStripModule : public AnalysisModule
{
    public:
        virtual void initialize() override;

        DataStripModule(std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> recomod, std::shared_ptr<LeptonJetMatchingModule> matchmod);

        virtual void finalize() override;
        

    private:
        TFile *file;
        TTree *tree;
        TTree *tree2;
        Float_t pt;
        Int_t jetIndex;
        Int_t nParticles; //? not jetIndex?
        Float_t phi;
        Float_t eta;
        Float_t mass;
        Float_t deltaR;

        const std::shared_ptr<LeptonJetReconstructionModule> recomod;
        const std::shared_ptr<LeptonJetMatchingModule> matchmod;

    protected:
        virtual bool process() override;

};

#endif