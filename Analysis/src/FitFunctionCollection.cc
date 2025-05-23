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
void FitFunctionCollection::saveFunctions(const std::string& fileName, bool append)
{
	std::fstream file;
	if (!file.is_open()) 
	{
		if (append) 
			file = std::fstream(fileName, std::ios_base::app);
		else
			file = std::fstream(fileName, std::ios_base::out);
	} 
	else 
	{
		throw std::invalid_argument(fileName + " file is already open");
	}

	file << functions.size() << '\n';
	for (auto& funcPair : functions)
	{
		file << funcPair.second << "\n";
	}
	file.close();
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

FitFunction& FitFunctionCollection::operator[](const std::string& key)
{
	return functions[key];
}

FitFunction& FitFunctionCollection::get(const std::string& key)
{
	try 
	{ 
		return functions.at(key);
	} 
	catch (std::out_of_range e)
	{
		std::cout << "FitFunctionCollection Error: No FitFunction with string key of " << key << '\n';
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

bool FitFunctionCollection::checkFunctionsSimilar()
{
	if (size() > 0)
	{
		FitFunction& compareFunc = functions.begin()->second;
		for (auto& pair : functions)
		{
			if (pair.second.getFunctionType() != compareFunc.getFunctionType()
			|| pair.second.getFunction()->GetNpar() != compareFunc.getFunction()->GetNpar())
			{
				return false;
			}
			else
			{
				for (int i = 0; i < compareFunc.getFunction()->GetNpar(); ++i)
				{
					std::string firstFunc(compareFunc.getFunction()->GetParName(i));
					std::string secondFunc(pair.second.getFunction()->GetParName(i));
					if (firstFunc != secondFunc)
					{
						std::cout << "compareFunc: " << compareFunc.getFunction()->GetParName(i) << '\n';
						std::cout << "currentFunc: " << pair.second.getFunction()->GetParName(i) << '\n';
						std::cout << "3\n";
						return false;
					}
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

std::unordered_map<std::string, FitFunction>& FitFunctionCollection::getFunctions()
{
	return functions;
}