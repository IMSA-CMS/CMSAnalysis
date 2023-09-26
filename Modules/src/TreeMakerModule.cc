#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"

void TreeMakerModule::initialize()
{
	addVariables();
}

bool TreeMakerModule::process()
{
	calculateVariables();
	return true;
}

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