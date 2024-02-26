#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include <string>
#include <fstream>

FitFunctionCollection FitFunctionCollection::loadFunctions(const std::string& fileName)
{
	std::fstream file(fileName, std::ios_base::in);
	if (file.is_open())
	{
		size_t size;
		file >> size;
		FitFunctionCollection functions(size);
		for (size_t i = 0; i < size; ++i)
		{
			FitFunction func;
			file >> func;
			functions.insert(func);
		}
		file.close();
		return functions;
	}
	else
	{
		throw std::runtime_error("File not loaded successfully");
	}
}
void FitFunctionCollection::saveFunctions(const std::string& fileName)
{
	std::fstream file(fileName, std::ios_base::out);
	if (file.is_open())
	{
		file << functions.size() << '\n';
		for (auto& funcPair : functions)
		{
			file << funcPair.second;
		}
		file.close();
	}
	else
	{
		throw std::runtime_error("File not saved successfully");
	}
}

FitFunctionCollection::FitFunctionCollection() {}

FitFunctionCollection::FitFunctionCollection(std::vector<FitFunction>& functions) 
{
	this->functions.reserve(functions.size());
	for (auto& func : functions)
	{
		this->insert(func);
	}
}

FitFunctionCollection::FitFunctionCollection(size_t size) 
{
	functions.reserve(size);
}

size_t FitFunctionCollection::size() const
{
	return functions.size();
}

FitFunction& FitFunctionCollection::operator[](const std::string& index)
{
	return functions[index];
}

FitFunction& FitFunctionCollection::get(const std::string& index)
{
	try 
	{ 
		return functions.at(index);
	} 
	catch (std::out_of_range e)
	{
		std::cout << "FitFunctionCollection Error: No FitFunction with string index of " << index << '\n';
		throw e;
	}
}

void FitFunctionCollection::insert(FitFunction& func)
{
	// std::cout << "Making function\n";
	// std::cout << "FuncName: " << func.getFunction()->GetName() << '\n';
	// std::cout << func;
	// // auto pair = std::make_pair<std::string, FitFunction>("test", func);
	// std::cout << "Pair";
	functions.insert({func.getFunction()->GetName(), func});
}

void FitFunctionCollection::insert(const std::string& key, FitFunction& func)
{
	functions.insert({key, func});
}
// FitFunction& FitFunctionCollection::operator[](int index)
// {
// 	return functions[(size_t) index];
// }