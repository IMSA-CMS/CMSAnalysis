#ifndef HIGGSDATASTRIPMODULE_H
#define HIGGSDATASTRIPMODULE_H

#include "AnalysisModule.hh"
#include "Rtypes.h"

class TFile;
class TTree;
class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

class HiggsDataStripModule : public AnalysisModule
{
public:
	virtual void initialize() override;

	HiggsDataStripModule(std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> recomod, std::shared_ptr<LeptonJetMatchingModule> matchmod);

	virtual void finalize() override;

	virtual std::string getName() override {return "HiggsDataStripModule";}

private:
	TFile *file;
	TTree *tree;
	Float_t pt1;
	Float_t pt2;
	Float_t phi1;
	Float_t phi2;
	Float_t eta1;
	Float_t eta2;
	Float_t deltaR;
	Float_t mass;

	const std::shared_ptr<LeptonJetReconstructionModule> recomod;
	const std::shared_ptr<LeptonJetMatchingModule> matchmod;

protected:
	virtual bool process() override;
};

#endif