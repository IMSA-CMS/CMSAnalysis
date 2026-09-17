#include "../interface/FitFunctionCollection.hh"
#include <TF1.h>
#include <fstream>
#include <string>

FitFunctionCollection FitFunctionCollection::loadFunctions(const std::string &fileName)
{
    std::ifstream file(fileName);
    
    if (file)
    {
        FitFunctionCollection functions;
        while (file)
        {
            std::cout << "Reading function #" << "..." << std::endl;
            SimpleFitFunction func(TF1(), FitFunction::FunctionType::ExpressionFormula);
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
        throw std::runtime_error("File " + fileName + " not loaded successfully");
    }

}

void FitFunctionCollection::saveFunctions(const std::string &fileName, bool append)
{
    std::ofstream file(fileName);
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

FitFunctionCollection::FitFunctionCollection()
{
}

FitFunctionCollection::FitFunctionCollection(std::vector<SimpleFitFunction> &functions)
{
    functions.reserve(functions.size());
    for (auto &func : functions)
    {
        insert(func);
    }
}

size_t FitFunctionCollection::size() const
{
    return functions.size();
}

SimpleFitFunction &FitFunctionCollection::operator[](const std::string &key)
{

    return get(key);
}

SimpleFitFunction &FitFunctionCollection::get(const std::string &key)
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

void FitFunctionCollection::insert(SimpleFitFunction func)
{
    functions.insert({func.getFunction()->GetName(), func});
}

void FitFunctionCollection::insert(const std::string& key, SimpleFitFunction func)
{
    functions.insert({key, func});
}

bool FitFunctionCollection::checkFunctionsSimilar()
{
    if (size() > 0)
    {
        SimpleFitFunction &compareFunc = functions.begin()->second;
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

std::unordered_map<std::string, SimpleFitFunction> &FitFunctionCollection::getFunctionsMap()
{
    return functions;
}

std::set<std::string> FitFunctionCollection::findUniqueNames(std::string parameter)
{
    std::set<std::string> result;
    for (auto& [key, fitFunction] : functions)
    {
        auto decoded = FitFunction::decodeName(fitFunction.getName());
        result.insert(decoded[parameter]);
    }
    return result;
}

FitFunctionCollection FitFunctionCollection::getFunctions(std::string name)
{
    FitFunctionCollection result;
    for (auto& [key, fitFunction] : functions)
    {
        if (fitFunction.getName().find(name) != std::string::npos)
        {
            result.insert(fitFunction);
        }
    }
    return result;
}

FitFunctionCollection FitFunctionCollection::getFunctions(std::string parameter, std::string name)
{
    FitFunctionCollection result;
    for (auto& [key, fitFunction] : functions)
    {
        auto decoded = FitFunction::decodeName(fitFunction.getName());
        if (decoded[parameter] == name)
        {
            result.insert(fitFunction);
        }
    }
    return result;
}

FitFunctionCollection& FitFunctionCollection::operator+=(const FitFunctionCollection& other)
{
    for (const auto& [key, func] : other.functions)
    {
        insert(key, func);
    }
    return *this;
}