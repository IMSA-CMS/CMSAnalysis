#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"


void LeptonJetMLStripModule::addVariables()
{
    addVariable("leadingPt", VariableType::Float);
    addVariable("nParticles", VariableType::Integer);
	addVariable("eta", VariableType::Float);
	//reader.AddVariable("phi", &phi);
	// reader.AddVariable("mass", &mass);
	addVariable("deltaR", VariableType::Float);
	addVariable("sumPt", VariableType::Float);
	addVariable("numMuons", VariableType::Integer);
	addVariable("deltaPt", VariableType::Float);

}

void LeptonJetMLStripModule::calculateVariables()
{
    addValue("numParticles", leptonjet.getNumParticles());
}
