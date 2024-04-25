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
	for (auto& pair : integers)
	{
		tempRef = (&(pair.second));
		reader->AddVariable((pair.first).c_str(), const_cast<Int_t*>(tempRef));
		std::cout << "temporary int reference" << *tempRef;
	}
	
	
	for (auto& pair : floats)
	{
		tempRef1 = (&(pair.second));
		reader->AddVariable((pair.first).c_str(), const_cast<Float_t*>(tempRef1)); // reinterpret_cast<Float_t*>
		std::cout << "temporary float reference" << *tempRef1;
	}
}

void SpecialVariableModule::addVariablesToTree(TTree *tree)
{
	for (auto& pair : integers)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		// auto ptm = &std::pair<std::string, int>::second;
		tree->Branch(pair.first.c_str(), reinterpret_cast<void*>(&(pair.second)), (pair.first+"/F").c_str());
	}
	
	
	for (auto& pair : floats)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		tree->Branch(pair.first.c_str(), reinterpret_cast<void*>(&(pair.second)), (pair.first+"/F").c_str());
	}

	//std::cout << "Address of numLeptons: " << &(integers.find("nParticles")->second) << '\n';
}

void SpecialVariableModule::addVariablesToDataLoader(TMVA::DataLoader *dataloader) const
{
	for (auto pair : integers)
	{
		std::cout << "Adding variable " << pair.first << '\n';
		dataloader->AddVariable(pair.first.c_str(), pair.first.c_str(), "", 'F');
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
	//calculateVariables();
	return true;
}

void SpecialVariableModule::addValue(std::string name, Int_t value)
{
	addValue(name, integers, value);
	//std::cout << "New Address of numLeptons: " << &(integers["nParticles"]) << '\n';
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
