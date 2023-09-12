#ifndef TREEMAKERMODULE_HH
#define TREEMAKERMODULE_HH

#include <unordered_map>

#include "ProductionModule.hh"

class TTree;
namespace TMVA
{
	class Reader;
}

class TreeMakerModule : public ProductionModule
{
public:
	enum VariableType {Integer, Float, IntegerArray, FloatArray};

	template<typename T>
	void addValue(std::string name, T type) = delete;

	template<typename T>
	T getValue(std::string name) = delete;

	TTree* makeTree();

	void addVariablesToReader(TMVA::Reader* reader);

	virtual bool process() override;

protected:
	void addVariable(std::string name, VariableType type);
	virtual void addVariables() = 0;

private:
	std::unordered_map<std::string, Int_t> integers;
	std::unordered_map<std::string, Float_t> floats;
	std::unordered_map<std::string, std::vector<Int_t>> arraysOfIntegers;
	std::unordered_map<std::string, std::vector<Float_t>> arraysOfFloats;
};

template<>
inline void TreeMakerModule::addValue(std::string name, Int_t type)
{
	integers[name] = type;
}

template<>
inline void TreeMakerModule::addValue(std::string name, Float_t type)
{
	floats[name] = type;
}

template<>
inline Int_t TreeMakerModule::getValue(std::string name)
{
	if (auto it = integers.find(name); it == integers.end())
	{
		throw std::runtime_error("Integer " + name + " not found in tree");
	}
	else
	{
		return it->second;
	}
}

template<>
inline Float_t TreeMakerModule::getValue(std::string name)
{
	if (auto it = floats.find(name); it == floats.end())
	{
		throw std::runtime_error("Float " + name + " not found in tree");
	}
	else
	{
		return it->second;
	}
}

#endif