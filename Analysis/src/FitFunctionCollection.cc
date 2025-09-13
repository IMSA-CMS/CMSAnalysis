#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include <fstream>
#include <string>

FitFunctionCollection FitFunctionCollection::loadFunctions(const std::string &fileName)
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
    //std::cout << "fstream object created";
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
    //std::cout << "if-block passed";

	std::map<std::string, std::vector<FitFunction>> channel_parameters;
	std::vector<std::string> channelNames;

	// file << functions.size() << '\n';

	for (auto& funcPair : functions)
	{
		std::string channel = funcPair.second.getChannelName();
		auto channelExists = std::find(channelNames.begin(), channelNames.end(), channel);
		if (channelExists == channelNames.end())
		{
			std::cout << channel << " does not exist in file \n";
			channelNames.push_back(channel);
			channel_parameters[channel] = {funcPair.second};
		}
		else
		{
			std::cout << channel << " exists in file \n";
			channel_parameters[channel].push_back(funcPair.second);
		}

		//file << funcPair.second << "\n";
	}

    std::cout << "Writing to file... \n";
	for (std::string channel : channelNames)
	{
		file << "Channel name: " << channel << '\n' << '\n';
		std::vector<FitFunction> functionParameters = channel_parameters.at(channel);
        std::cout << "Function parameters retrieved \n";
		file << "Calculated Parameters" << "		";
		
		// Get labels for parameters like p0, p1, etc. from first function in functionParameters
		auto firstFunction = channel_parameters.at(channel)[0].getFunction();
		for (int i = 0; i < firstFunction->GetNpar(); ++i)
		{
			file << firstFunction->GetParName(i) << "		";
		}
		for (int i = 0; i < firstFunction->GetNpar(); ++i)
		{
			file << firstFunction->GetParName(i) << "_error" << "		";
		}

        file << '\n';

		// List out the functions with their values for each parameter
		for (auto& parameter : functionParameters)
		{
			file << parameter;
		}

		file << '\n' << '\n' << '\n';


	}

	file.close();
}




// void FitFunctionCollection::saveFunctions(const std::string& fileName, bool append)
// {
// 	std::fstream file;
// 	if (!file.is_open()) 
// 	{
// 		if (append) 
// 			file = std::fstream(fileName, std::ios_base::app);
// 		else
// 			file = std::fstream(fileName, std::ios_base::out);
// 	} 
// 	else 
// 	{
// 		throw std::invalid_argument(fileName + " file is already open");
// 	}

// 	file << functions.size() << '\n';
// 	for (auto& funcPair : functions)
// 	{
// 		file << funcPair.second << "\n";
// 	}
// 	file.close();
// }

FitFunctionCollection::FitFunctionCollection() {}

FitFunctionCollection::FitFunctionCollection(std::vector<FitFunction> &functions)
{
    this->functions.reserve(functions.size());
    for (auto &func : functions)
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

FitFunction &FitFunctionCollection::operator[](const std::string &key)
{
    return functions[key];
}

FitFunction &FitFunctionCollection::get(const std::string &key)
{
    try
    {
        return functions.at(key);
    }
    catch (std::out_of_range& e)
    {
        std::cout << "FitFunctionCollection Error: No FitFunction with string key of " << key << '\n';
        throw e;
    }
}

void FitFunctionCollection::insert(FitFunction &func)
{
    // std::cout << "Making function\n";
    // std::cout << "FuncName: " << func.getFunction()->GetName() << '\n';
    // std::cout << func;
    // // auto pair = std::make_pair<std::string, FitFunction>("test", func);
    // std::cout << "Pair";
    functions.insert({func.getFunction()->GetName(), func});
}

void FitFunctionCollection::insert(const std::string &key, FitFunction &func)
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
        FitFunction &compareFunc = functions.begin()->second;
        for (auto &pair : functions)
        {
            if (pair.second.getFunctionType() != compareFunc.getFunctionType() ||
                pair.second.getFunction()->GetNpar() != compareFunc.getFunction()->GetNpar())
            {
                if (pair.second.getFunctionType() != compareFunc.getFunctionType())
                {
                    std::cout << "Functions are different types\n";
                }
                else
                {
                    std::cout << "Functions have different Npar\n";
                }

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
        std::cout << "size is not greater than 0\n";
        return false;
    }
}

std::unordered_map<std::string, FitFunction> &FitFunctionCollection::getFunctions()
{
    return functions;
}