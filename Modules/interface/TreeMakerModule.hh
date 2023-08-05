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
	void addValue(std::string name, T type);

	template<typename T>
	T getValue(std::string name);

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

template<typename T>
inline void addValue(std::string name, T type)
{

}

template<typename T>
T getValue(std::string name)
{

}

#endif