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


void TreeMakerModule::initialize()
{
	addVariables();
}

bool TreeMakerModule::process()
{
	calculateVariables();
	return true;
}
<<<<<<< HEAD
=======

void TreeMakerModule::addValue(std::string name, Int_t value)
{
	addValue(name, integers, value);
}

void TreeMakerModule::addValue(std::string name, Float_t value)
{
	addValue(name, floats, value);
}

void TreeMakerModule::addValue(std::string name, std::vector<Int_t> value)
{
	addValue(name, arraysOfIntegers, value);
}

void TreeMakerModule::addValue(std::string name, std::vector<Float_t> value)
{
	addValue(name, arraysOfFloats, value);
}

void TreeMakerModule::addVariable(std::string name, VariableType type)
{
	switch (type)
	{
		case VariableType::Integer:
			addVariable(name, integers);
			break;
		
		case VariableType::Float:
			addVariable(name, floats);
			break;

		case VariableType::IntegerArray:
			addVariable(name, arraysOfIntegers);
			break;

		case VariableType::FloatArray:
			addVariable(name, arraysOfFloats);
			break;

		default:
			throw std::runtime_error("Unknown variable type");
	}
}
>>>>>>> 52b4a792fa62be4f2d344ab0915857c689bf9b0b
