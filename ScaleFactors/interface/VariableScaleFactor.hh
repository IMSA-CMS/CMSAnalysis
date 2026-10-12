#ifndef VARIABLESCALEFACTOR_HH
#define VARIABLESCALEFACTOR_HH
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class VariableScaleFactor : public ScaleFactor
{
    
public:
    VariableScaleFactor(std::string iname, std::string variableNameNominal, std::string variableNameUp, std::string variableNameDown);
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;

private:
    std::string variableNameNominal;
    std::string variableNameUp;
    std::string variableNameDown;
};

#endif // SCALEFACTOR_HH

// write source file for this class and then variables are in the nanoAOD event file, so we can get them from there. We can also add more variables in the future if we want to. We can also add more systematic types if we want to.
