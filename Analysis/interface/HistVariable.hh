#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include "CMSAnalysis/Analysis/interface/FilePathMapper.hh"

#include <string>
#include <unordered_map>
#include <stdexcept>

class FilePathMapper 
{
public:
    // Add or update a mapping
    void addMapping(const std::string& variableName, const std::string& filePath) 
    {
        mappings[variableName] = filePath;
    }

    // Get the file path for a variable
    std::string getFilePath(const std::string& variableName) const 
    {
        auto it = mappings.find(variableName);
        if (it == mappings.end()) 
        {
            throw std::runtime_error("File path for variable '" + variableName + "' not found in mappings.");
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, std::string> mappings;
};

class HistVariable 
{
public:
    // Static objects for predefined variables
    static HistVariable genSimSameSignMass;
    static HistVariable sameSignMass;
    static HistVariable invariantMass;
    static HistVariable genSimPt;
    static HistVariable pt;
    static HistVariable eta;
    static HistVariable phi;
    static HistVariable mET;
    static HistVariable firstPt;
    static HistVariable secondPt;
    static HistVariable thirdPt;
    static HistVariable fourthPt;

    // Getters
    std::string getName() const { return name; }
    std::string getGraphName(const FilePathMapper& mapper) const 
    {
        return mapper.getFilePath(name);
    }

    // Constructor
    HistVariable(std::string iName) : name(std::move(iName)) {}

private:
    std::string name;
};

#endif
