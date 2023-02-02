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

    virtual void writeAll() override;

    virtual void finalize() override;

private:
    TFile *file;
    TTree *tree;
    TTree *tree2;
    Float_t leading_pt;
    Int_t nParticles;
    Float_t phi;
    Float_t eta;
    Float_t deltaR;
    Float_t sum_pt;
    Float_t delta_pt;

    const std::shared_ptr<LeptonJetReconstructionModule> recomod;
    const std::shared_ptr<LeptonJetMatchingModule> matchmod;

protected:
    virtual bool process() override;
};

#endif