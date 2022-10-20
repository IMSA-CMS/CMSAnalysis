#ifndef LEPTONJETMLCALCULATOR_H
#define LEPTONJETMLCALCULATOR_H

#include "ProductionModule.hh"
#include "Rtypes.h"
#include "TMVA/Reader.h"

class LeptonJetReconstructionModule;
class LeptonJetMatchingModule;

class LeptonJetMLCalculator : public ProductionModule
{
private:
	mutable Float_t pt;
	mutable Float_t phi;
	mutable Float_t eta;
	// mutable Float_t mass;
	mutable Float_t deltaR;
	mutable Float_t nParticles;
	// mutable Int_t jetIndex;
	mutable TMVA::Reader reader;

public:
	virtual void initialize() override;
	LeptonJetMLCalculator();
	virtual double CalculateMLValue(LeptonJet jet) const;
	virtual bool process() override { return true; }
};

#endif