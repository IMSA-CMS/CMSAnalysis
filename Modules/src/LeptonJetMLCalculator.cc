#include "CMSAnalysis/Modules/interface/LeptonJetMLCalculator.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "TMVA/Reader.h"
#include <time.h>

LeptonJetMLCalculator::LeptonJetMLCalculator() : reader("!Color:!Silent:!V")
{
	reader.AddVariable("leadingPt", &leadingPt);
	reader.AddVariable("nParticles", &nParticles);
	reader.AddVariable("eta", &eta);
	//reader.AddVariable("phi", &phi);
	// reader.AddVariable("mass", &mass);
	reader.AddVariable("deltaR", &deltaR);
	reader.AddVariable("sumPt", &sumPt);
	reader.AddVariable("numMuons", &numMuons);
	reader.AddVariable("deltaPt", &deltaPt);
	// reader.AddSpectator("spec1 := jetIndex*2", &jetIndex);
	reader.BookMVA("MLP", "dataset/weights/TMVAClassification_MLP.weights.xml");
}

void LeptonJetMLCalculator::initialize()
{
}

double LeptonJetMLCalculator::CalculateMLValue(LeptonJet jet) const
{
	sumPt = 0;
	nParticles = jet.getNumParticles();
	phi = jet.getFourVector().Phi();
	eta = jet.getFourVector().Eta();
	// mass = jet.getFourVector().mass();
	deltaR = 0;
	// jetIndex = 0; // later retrain and rewrite ReadJet.C without this spectator variable
	leadingPt = 0;
	double runnerUpPt = 0;
	deltaPt = 0;
	numMuons = 0;


	for (Particle p : jet.getParticles())
	{
		for (Particle q : jet.getParticles())
		{
			if (p.getDeltaR(q) > deltaR)
			{
				deltaR = p.getDeltaR(q);
			}
		}
		if (p.getPt() > leadingPt)
		{
			leadingPt = p.getPt();
		}
		sumPt += p.getPt();
		
		if (p.getType() == ParticleType::muon())
		{
			numMuons++;
		}
	}
	deltaPt = leadingPt - runnerUpPt;

	//std::cout << "leadingPt " << leadingPt << "\n";
	//std::cout << "nparticles " << nParticles << "\n";
	//std::cout << "eta " << eta << "\n";
	//std::cout << "deltaR " << deltaR << "\n";
	//std::cout << "sumPt " << sumPt << "\n";
	//std::cout << "numMuons " << numMuons << "\n";
	//std::cout << "deltapt " << deltaPt << "\n";
	auto value = reader.EvaluateMVA("MLP");
	std::cout << "numchange " << value << "\n";
	return value;
}