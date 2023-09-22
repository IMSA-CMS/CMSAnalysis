#ifndef TREEMAKERMODULE_HH
#define TREEMAKERMODULE_HH

#include <unordered_map>
#include "Rtypes.h"
#include "TMVA/Reader.h"
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
	T getValue(std::string name) const;

	void addVariablesToReader(TMVA::Reader* reader) const; //basically MLcalc //assume map has been created // useforreach loop

	void addVariablesToTree(TTree* tree) const; //same thing as prior just datastrip stuff

	

	virtual bool process() override;

protected:
	void addVariable(std::string name, VariableType type);

	virtual void calculateVariables() = 0;

	virtual void addAllVariables() = 0; //call in contrcutor //this calls addvariable

private:
	std::unordered_map<std::string, Int_t> integers;
	std::unordered_map<std::string, Float_t> floats;
	std::unordered_map<std::string, std::vector<Int_t>> arraysOfIntegers;
	std::unordered_map<std::string, std::vector<Float_t>> arraysOfFloats;
};

template<typename T>
inline void TreeMakerModule::addValue(std::string name, T type)
{

}

template<typename T>
T TreeMakerModule::getValue(std::string name) const
{

}
//make tree, add =variables to reader

#endif