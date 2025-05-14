#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include "CMSAnalysis/Analysis/interface/FilePathMapper.hh"

#include <string>
#include <unordered_map>
#include <stdexcept>

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
    bool is2DHistX = false;
    bool is2DHistY = false;
    bool getis2dHistX() {return is2DHistX;}
    bool getis2dHistY() {return is2DHistY;}

private:
    std::string name;
};

#endif
