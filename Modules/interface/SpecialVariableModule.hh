#ifndef SPECIALVARIABLEMODULE_HH
#define SPECIALVARIABLEMODULE_HH

#include <map>
#include "Rtypes.h"
#include "ProductionModule.hh"
#include "EventInput.hh"

class TTree;
namespace TMVA
{
	class Reader;
	class DataLoader;
}

class SpecialVariableModule : public ProductionModule
{
public:
	enum VariableType {Integer, Float, IntegerArray, FloatArray};

	virtual void initialize() override;

	void addValue(std::string name, Int_t value);
	void addValue(std::string name, Float_t value);
	void addValue(std::string names, double value);
	void addValue(std::string name, std::vector<Int_t> value);
	void addValue(std::string name, std::vector<Float_t> value);

	void addVariablesToTree(TTree* tree); //same thing as prior just datastrip stuff
	void addVariablesToReader(TMVA::Reader *reader) const;
	void addVariablesToDataLoader(TMVA::DataLoader *dataloader) const;


	virtual bool process() override;

	virtual void calculateVariables(ParticleCollection<Particle> particle) = 0;

	mutable const int* tempRef;
	mutable const float* tempRef1;

	virtual std::string getName() override {return "SpecialVariableModule";}

protected:
	void addVariable(std::string name, VariableType type);
	
	virtual void addVariables() = 0;

private:
	std::map<std::string, Int_t> integers;
	std::map<std::string, Float_t> floats;
	std::map<std::string, std::vector<Int_t>> arraysOfIntegers;
	std::map<std::string, std::vector<Float_t>> arraysOfFloats;

	TTree* tree = nullptr;
	
	template<typename T>
	void addValue(std::string name,
		std::map<std::string, T, std::less<std::string>, std::allocator<std::pair<const std::string, T>>>& map,
		T value);

	template<typename T>
	void addVariable(std::string name,
		std::map<std::string, T, std::less<std::string>, std::allocator<std::pair<const std::string, T>>>& map);
};

template<typename T>
inline void SpecialVariableModule::addValue(std::string name, 
	std::map<std::string, T, std::less<std::string>, std::allocator<std::pair<const std::string, T>>>& map,
	T value)
{
	map[name] = value;

	//auto pair = map.find(name);
	//std::cout << "For variable " << name << " storing value " << pair->second << " at memory address "
	//<< &(pair->second) << '\n'; 
}

template<typename T>
inline void SpecialVariableModule::addVariable(std::string name,
	std::map<std::string, T, std::less<std::string>, std::allocator<std::pair<const std::string, T>>>& map)
{
	if (map.find(name) != map.end())
	{
		throw std::runtime_error("Variable " + name + " already exists");
	}
	map.insert({name, T()}); // Creates a new one if it doesn't exist
}
//make tree, add =variables to reader

#endif