#include "../interface/FitFunctionBase.hh"
#include <sstream>
#include <utility>

FitFunctionBase::FitFunctionBase(const FunctionType functionType, std::string name)
    : functionType(functionType), name(std::move(name))
{
}

void FitFunctionBase::setName(std::string newName)
{
    name = std::move(newName);
}

std::string FitFunctionBase::encodeName(std::map<std::string, std::string> parameters)
{
    std::string result;
    for (auto &[key, value] : parameters)
    {
        result += key + " - " + value + " | ";
    }
    return result;
}

std::map<std::string, std::string> FitFunctionBase::decodeName(std::string name)
{
    std::map<std::string, std::string> result;
    std::istringstream stream(std::move(name));
    std::string token;
    while (std::getline(stream, token, '|'))
    {
        const size_t dashPos = token.find(" - ");
        if (dashPos == std::string::npos)
        {
            continue;
        }
        std::string key = token.substr(0, dashPos);
        std::string value = token.substr(dashPos + 3);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        result[key] = value;
    }
    return result;
}
