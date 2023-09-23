#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"
#include "TFile.h"
#include "TTree.h"


bool TreeMakerModule::process()
{
	calculateVariables();

	return true;
}

//void addVariable(TMVA::Reader* reader, std::string name, VariableType type)
//{
//	reader.AddVariable("leadingPt", &leadingPt);
//}

void TreeMakerModule::addVariablesToReader(TMVA::Reader *reader) const
{
	for (auto pair : integers)
	{
		reader->AddVariable(pair.first, &pair.second);
	}
	
	
	for (auto pair : floats)
	{
		reader->AddVariable(pair.first, &pair.second);
	}
}

void TreeMakerModule::addVariablesToTree(TTree *tree) const
{
	for (auto pair : integers)
	{
		tree->Branch(pair.first.c_str(), &pair.second, (pair.first+"/I").c_str());
	}
	
	
	for (auto pair : floats)
	{
		tree->Branch(pair.first.c_str(), &pair.second, (pair.first+"/F").c_str());
	}
}


bool TreeMakerModule::process()
{
	addVariables();
	return true;
}
