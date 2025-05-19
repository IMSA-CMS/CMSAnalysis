#ifndef HISTVARIABLESELECTOR_HH
#define HISTVARIABLESELECTOR_HH

#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <stdexcept>
#include <string>

class HistVariableSelector
{
public:
    // Enum for reco level
    enum class RecoLevel
    {
        Reco,
        GenSim
    };

    // Enum for variable types
    enum class VariableType
    {
        SameSignMass,
        InvariantMass,
        Pt,
        Eta,
        Phi,
        MET,
        FirstPt,
        SecondPt,
        ThirdPt,
        FourthPt
    };

    // Constructor
    HistVariableSelector(RecoLevel recoLevel, VariableType variableType)
        : recoLevel(recoLevel), variableType(variableType) {}

    // Setters
    static void setRecoLevel(RecoLevel level)
    {
        recoLevel = level;
    }

    static void setVariableType(VariableType type)
    {
        variableType = type;
    }

    // Get the selected HistVariable based on reco level and variable type
    static const HistVariable& getSelectedVariable() const
    {
        if (recoLevel == RecoLevel::GenSim)
        {
            switch (variableType)
            {
            case VariableType::SameSignMass:
                return HistVariable::genSimSameSignMass;
            case VariableType::Pt:
                return HistVariable::genSimPt;
            default:
                throw std::runtime_error("GenSim does not support this variable type.");
            }
        }
        else // Reco level
        {
            switch (variableType)
            {
            case VariableType::SameSignMass:
                return HistVariable::sameSignMass;
            case VariableType::InvariantMass:
                return HistVariable::invariantMass;
            case VariableType::Pt:
                return HistVariable::pt;
            case VariableType::Eta:
                return HistVariable::eta;
            case VariableType::Phi:
                return HistVariable::phi;
            case VariableType::MET:
                return HistVariable::mET;
            case VariableType::FirstPt:
                return HistVariable::firstPt;
            case VariableType::SecondPt:
                return HistVariable::secondPt;
            case VariableType::ThirdPt:
                return HistVariable::thirdPt;
            case VariableType::FourthPt:
                return HistVariable::fourthPt;
            default:
                throw std::runtime_error("Reco does not support this variable type.");
            }
        }
    }

    // Get the name of the selected HistVariable
    std::string getSelectedVariableName() const
    {
        return getSelectedVariable().getName();
    }

    // Get the graph name of the selected HistVariable
    std::string getSelectedGraphName(const FilePathMapper& mapper) const
    {
        return getSelectedVariable().getGraphName(mapper);
    }

private:
    RecoLevel recoLevel;
    VariableType variableType;
};

#endif