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

protected:
	void addVariable(std::string name, VariableType type);

private:
	std::unordered_map<std::string, int> indices;
	std::vector<Int_t> integers;
	std::vector<Float_t> floats;
	std::vector<std::vector<Int_t>> arraysOfIntegers;
	std::vector<std::vector<Float_t>> arraysOfFloats;
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