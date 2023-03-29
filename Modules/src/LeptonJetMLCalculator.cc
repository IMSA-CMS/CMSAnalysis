#include "CMSAnalysis/DataCollection/interface/LeptonJetMLCalculator.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "TMVA/Reader.h"
#include <time.h>

LeptonJetMLCalculator::LeptonJetMLCalculator() : reader("!Color:!Silent:!V")
{
	reader.AddVariable("pt", &pt);
	reader.AddVariable("nParticles", &nParticles);
	reader.AddVariable("eta", &eta);
	reader.AddVariable("phi", &phi);
	// reader.AddVariable("mass", &mass);
	reader.AddVariable("deltaR", &deltaR);
	// reader.AddSpectator("spec1 := jetIndex*2", &jetIndex);
	reader.BookMVA("MLP", "dataset/weights/TMVAClassification_MLP.weights.xml");
}

void LeptonJetMLCalculator::initialize()
{
}

double LeptonJetMLCalculator::CalculateMLValue(LeptonJet jet) const
{
	pt = jet.getFourVector().Pt();
	nParticles = jet.getNumParticles();
	phi = jet.getFourVector().Phi();
	eta = jet.getFourVector().Eta();
	// mass = jet.getFourVector().mass();
	deltaR = 0;
	// jetIndex = 0; // later retrain and rewrite ReadJet.C without this spectator variable
	for (Particle p : jet.getParticles())
	{
		for (Particle q : jet.getParticles())
		{
			if (p.getDeltaR(q) > deltaR)
			{
				deltaR = p.getDeltaR(q);
			}
		}
	}
	auto value = reader.EvaluateMVA("MLP");
	return value;
}