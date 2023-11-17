#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"
#include "TFile.h"
#include "TTree.h"
#include "TMVA/Reader.h"
#include "TMVA/DataLoader.h"



//void addVariable(TMVA::Reader* reader, std::string name, VariableType type)
//{
//	reader.AddVariable("leadingPt", &leadingPt);
//}

void SpecialVariableModule::addVariablesToReader(TMVA::Reader *reader) const
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

void SpecialVariableModule::addVariablesToTree(TTree *tree) const
{
	for (auto pair : integers)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		tree->Branch(pair.first.c_str(), &pair.second, (pair.first+"/I").c_str());
	}
	
	
	for (auto pair : floats)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		tree->Branch(pair.first.c_str(), &pair.second, (pair.first+"/F").c_str());
	}
}

void SpecialVariableModule::addVariablesToDataLoader(TMVA::DataLoader *dataloader) const
{
	for (auto pair : integers)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		dataloader->AddVariable(pair.first.c_str(), pair.first.c_str(), "", 'I');
	}
	
	
	for (auto pair : floats)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		dataloader->AddVariable(pair.first.c_str(), pair.first.c_str(), "", 'F');
	}
}

void SpecialVariableModule::initialize()
{
	std::cout << "Initializing ";
	addVariables();
	for (auto val : floats)
	{
		std::cout << "Entry key " << val.first << '\n';
	}
}

bool SpecialVariableModule::process()
{
	calculateVariables();
	return true;
}

void SpecialVariableModule::addValue(std::string name, Int_t value)
{
	addValue(name, integers, value);
}

void SpecialVariableModule::addValue(std::string name, Float_t value)
{
	addValue(name, floats, value);
}

void SpecialVariableModule::addValue(std::string name, std::vector<Int_t> value)
{
	addValue(name, arraysOfIntegers, value);
}

void SpecialVariableModule::addValue(std::string name, std::vector<Float_t> value)
{
	addValue(name, arraysOfFloats, value);
}

void SpecialVariableModule::addValue(std::string name, double value)

{
	addValue(name, static_cast<Float_t>(value));
}

void SpecialVariableModule::addVariable(std::string name, VariableType type)
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
