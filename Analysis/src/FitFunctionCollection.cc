#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

void addToChannelModifierMap(std::string newKey, 
    std::string newValue, 
    std::map<std::string,std::string>* map, 
    std::vector<std::string>* keysInMap);

std::vector<FitFunction> alphabetizeParameters(std::vector<FitFunction> fitFunctions);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string replaceSubstring(std::string stringToModify, const std::string from, const std::string to);

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

    // Loads in possible modifiers for the channel names (Ex: to differentiate between X and Y)
    // Key: Modifier in parameters or some other place
    // Value: What you want to add to the name of the channelName
    std::map<std::string, std::string> channelNameModifiers = {};
    std::vector<std::string> keysInChannelNameModifiers = {};

    // Here is a template for adding new Keys to channelNameModifers:
    // addToStringVectorMap("newKey", "newValue", &channelNameModifiers, &keysInChannelNameModifiers);
    addToChannelModifierMap("mll1", "_X", &channelNameModifiers, &keysInChannelNameModifiers);
    addToChannelModifierMap("mll2", "_Y", &channelNameModifiers, &keysInChannelNameModifiers);

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
        std::string unmodifiedChannelName = funcPair.second.getChannelName();
		std::string channel = replaceSubstring(unmodifiedChannelName, "m", "u"); // Replace "m" with "u" for muons
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

    // Alphabetize parameters for each channel
    for (auto channel : channelNames)
    {
        channel_parameters[channel] = alphabetizeParameters(channel_parameters[channel]);
    }

    std::cout << "Writing to file... \n";
	for (std::string channel : channelNames)
	{
        // Get labels for any channel name modifiers, parameters like p0, p1, etc. from first function in functionParameters
		auto firstFunction = channel_parameters.at(channel)[0].getFunction();
        std::string x_YNameModifier = ""; 
        for (auto modifier : keysInChannelNameModifiers)
        {
            if (std::string(firstFunction->GetName()).find(modifier) != std::string::npos)
            {
                x_YNameModifier = modifier;
            }
        }

        file << "Channel name: " << channel + channelNameModifiers[x_YNameModifier] << '\n' << '\n';
		std::vector<FitFunction> functionParameters = channel_parameters.at(channel);
        if (functionParameters.size() != 0)
        {
            std::cout << "Function parameters retrieved \n";

        }
        else
        {
            std::cout << "Warning: no function parameters retrieved \n";
        }
        //std::cout << "Function parameters retrieved \n";
		file << "Calculated Parameters" << "		";
		
		// // Get labels for parameters like p0, p1, etc. from first function in functionParameters
		// auto firstFunction = channel_parameters.at(channel)[0].getFunction();
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

// Replaces all occurrences of 'from' with 'to' in the input string
std::string replaceSubstring(std::string stringToModify, const std::string from, const std::string to) 
{
    std::string str = stringToModify;

    // Avoid infinite loop
    if (from.empty()) 
    {
        return from;
    }

    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Advance position to avoid replacing newly inserted 'to'
    }

    return str;
}

std::vector<FitFunction> alphabetizeParameters(std::vector<FitFunction> fitFunctions)
{
    std::vector<std::string> parameterNames;
    std::vector<FitFunction> alphabetizedFunctions;
    
    // Find and alphabetize the names of fit functions
    for (auto function : fitFunctions)
    {
        parameterNames.push_back(function.getParameterName());
    }
    std::sort(parameterNames.begin(), parameterNames.end());


    // Reorder the fitFunctions based on the order of parameterNames
    for (auto parameterName : parameterNames)
    {
        for (auto function : fitFunctions)
        {
            
            if (function.getParameterName() == parameterName)
            {
                alphabetizedFunctions.push_back(function);
            }
        }
    }

    return alphabetizedFunctions;
}

// Helper function for splitting strings
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}


// ONLY use this to add new key/value pairs to channelNameModifiers in saveFunctions 
// and keep track of keys 
void addToChannelModifierMap(std::string newKey, 
    std::string newValue, 
    std::map<std::string,std::string>* map, 
    std::vector<std::string>* keysInMap)
{
    (*map)[newKey] = newValue;
    if (std::find(keysInMap->begin(), keysInMap->end(), newKey) == keysInMap->end())
    {
        keysInMap->push_back(newKey);
    }

}

// void removeFromStringVectorMap()
// {
    
// }


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