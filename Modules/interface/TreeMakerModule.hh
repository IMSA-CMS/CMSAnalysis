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

	virtual void initialize() override;

<<<<<<< HEAD
	template<typename T>
	T getValue(std::string name) const = delete;

	void addVariablesToReader(TMVA::Reader* reader) const; //basically MLcalc //assume map has been created // useforreach loop
=======
	void addValue(std::string name, Int_t value);
	void addValue(std::string name, Float_t value);
	void addValue(std::string name, std::vector<Int_t> value);
	void addValue(std::string name, std::vector<Float_t> value);
>>>>>>> 52b4a792fa62be4f2d344ab0915857c689bf9b0b

	void addVariablesToTree(TTree* tree) const; //same thing as prior just datastrip stuff

	

	virtual bool process() override;

protected:
	void addVariable(std::string name, VariableType type);
	
	virtual void addVariables() = 0;

	virtual void calculateVariables() = 0;

private:
	std::unordered_map<std::string, Int_t> integers;
	std::unordered_map<std::string, Float_t> floats;
	std::unordered_map<std::string, std::vector<Int_t>> arraysOfIntegers;
	std::unordered_map<std::string, std::vector<Float_t>> arraysOfFloats;

	template<typename T>
	void addValue(std::string name,
		std::unordered_map<std::string, T, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, T>>> map,
		T value);

	template<typename T>
	void addVariable(std::string name,
		std::unordered_map<std::string, T, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, T>>> map);
};

template<typename T>
inline void TreeMakerModule::addValue(std::string name, 
	std::unordered_map<std::string, T, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, T>>> map,
	T value)
{
	if (map.find(name) != map.end())
	{
		throw std::runtime_error("Variable " + name + " already exists");
	}
	map[name] = value;
}

template<typename T>
inline void TreeMakerModule::addVariable(std::string name,
	std::unordered_map<std::string, T, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, T>>> map)
{
	map[name]; // Creates a new one if it doesn't exist
}
//make tree, add =variables to reader

#endif