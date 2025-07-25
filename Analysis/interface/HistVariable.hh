#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include "CMSAnalysis/Analysis/interface/FilePathMapper.hh"

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

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

    // Constructor
    HistVariable(std::string iName) : name(std::move(iName)) {}

    bool is2DHistX() {return is2DHistX_;}
    bool is2DHistY() {return is2DHistY_;}

    void setSystematic(ScaleFactor::SystematicType itype, std::string isystematicName) {type = itype; systematicName = isystematicName;}
    ScaleFactor::SystematicType getSystematicType() const {return type;}
    std::string getSystematicName() const {return systematicName;}

private:
    std::string name;
    bool is2DHistX_ = false;
    bool is2DHistY_ = false;
    ScaleFactor::SystematicType type = ScaleFactor::SystematicType::Nominal;
    std::string systematicName;

};

#endif
