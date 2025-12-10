#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include <TF1.h>
#include <fstream>
#include <string>
//run HiggsSignalFit make sure it works with adjustments




std::vector<FitFunction> alphabetizeParameters(std::vector<FitFunction> fitFunctions);
//std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to);

FitFunctionCollection FitFunctionCollection::loadFunctions(const std::string &fileName)
{
    std::ifstream file(fileName);
    
    if (file)
    {
        //size_t size;
        //file >> size;
        //std::cout << "First number read (expected size): " << size << std::endl;
        FitFunctionCollection functions;
        //for (size_t i = 0; i < size; ++i)
        while (file)
        {
            std::cout << "Reading function #" << "..." << std::endl;
            FitFunction func(TF1(), FitFunction::FunctionType::ExpressionFormula, "");
            file >> func;
            if (!file)
            {
                break;
            }
            functions.insert(func);
        }
        return functions;
    }
    else
    {
        throw std::runtime_error("File not loaded successfully");
    }

}
// FitFunctionCollection FitFunctionCollection::loadFunctions(const std::string &fileName)
// {
//     std::fstream file(fileName, std::ios_base::in);
//     if (!file)
//     {
//         throw std::runtime_error("File not loaded successfully: " + fileName);
//     }

//     std::cout << "Opening file: " << fileName << std::endl;

//     size_t size;
//     file >> size;
//     std::cout << "Declared number of functions (from file): " << size << std::endl;

//     FitFunctionCollection functions;

//     for (size_t i = 0; i < size; ++i)
//     {
//         std::cout << "Reading function #" << (i + 1) << std::endl;

//         FitFunction func;
//         if (!(file >> func))
//         {
//             std::cerr << "Failed to read FitFunction #" << (i + 1)
//                       << " at file position " << file.tellg() << ".\n";
//             break; // keep original behavior
//         }

//         auto tf1 = func.getFunction();
//         if (!tf1)
//         {
//             std::cerr << "TF1 is null for function #" << (i + 1) << ". Skipping.\n";
//             continue;
//         }

//         std::string name = tf1->GetName();
//         std::cout << "Inserting function: " << name << std::endl;

//         functions.insert(func);
//     }

//     std::cout << "Finished reading functions.\n";
//     std::cout << "Declared size: " << size
//               << ", Actually inserted: " << functions.size() << "\n";

//     return functions;
// }

void FitFunctionCollection::saveFunctions(const std::string &fileName, bool append)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file)
    {
        throw std::invalid_argument("File " + fileName + " not found!");
    }

    //file << functions.size() << '\n';
    for (auto &funcPair : functions)
    {
        file << funcPair.second << "\n";
    }
}

std::map<std::string, std::vector<FitFunction>> FitFunctionCollection::getFunctionsSortedByChannel(std::string fileName)
{
    std::map<std::string, std::vector<FitFunction>> sortedFunctions = {};
    std::vector<std::string> channelNames = {};

    std::map<std::string, std::string> channelNameModifiers = {};
    channelNameModifiers["mll1"] = "_X";
    channelNameModifiers["mll2"] = "_Y";


    std::vector<std::string> existingChannelNameModifiers = {};
    for (auto [modifier, replacement] : channelNameModifiers)
    {
        existingChannelNameModifiers.push_back(modifier);
    }


    FitFunctionCollection functionCollection;
    try {
        functionCollection = FitFunctionCollection::loadFunctions(fileName);
        std::cout << "Successfully read " << fileName << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load functions: " << e.what() << std::endl;
        throw;
    }


    // Sorts functions by channel
    auto parameterFunctions = functionCollection.getFunctions();
    for (auto [functionName, function] : parameterFunctions)
    {
        std::string channel = function.getChannel();
        //std::string channel = replaceAll(unformattedChannel, "m", "u");
        std::cout << "DEBUG: channel name for function: " << channel << "\n";
        // Check whether channel name needs to be modified for internal processes
        for (auto modifier : existingChannelNameModifiers)
        {
            if (function.getParameterName().find(modifier) != std::string::npos)
            {
                channel += channelNameModifiers[modifier];
            }
        }

        sortedFunctions[channel].push_back(function);

        // Record channels
        // if (std::find(channelNames.begin(), channelNames.end(), channel) == channelNames.end())
        // {
        //     channelNames.push_back(channel);
        // }
    }

    // Alphabetize functions for each channel
    // for (std::string channelName : channelNames)
    // {
    //     std::vector<FitFunction> alphabetizedFunctions = alphabetizeParameters(sortedFunctions[channelName]);
    //     sortedFunctions[channelName] = alphabetizedFunctions;
    // }

    return sortedFunctions;
}

// std::string replaceAll(std::string unmodifiedString, const std::string from, const std::string to)
// {
//     if (from.empty())
//         return unmodifiedString; // avoid infinite loop if 'from' is empty

//     std::string str = unmodifiedString;
//     size_t start_pos = 0;
//     while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
//         str.replace(start_pos, from.length(), to);
//         start_pos += to.length(); // move past the replaced part
//     }
    
//     return str;

// }

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




FitFunctionCollection::FitFunctionCollection()
{
}

FitFunctionCollection::FitFunctionCollection(std::vector<FitFunction> &functions)
{
    this->functions.reserve(functions.size());
    for (auto &func : functions)
    {
        this->insert(func);
    }
}

// FitFunctionCollection::FitFunctionCollection(size_t size)
// {
//     functions.reserve(size);
// }

size_t FitFunctionCollection::size() const
{
    return functions.size();
}

FitFunction &FitFunctionCollection::operator[](const std::string &key)
{

    return get(key);
}

FitFunction &FitFunctionCollection::get(const std::string &key)
{
    try
    {
        return functions.at(key);
    }
    catch (std::out_of_range &e)
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