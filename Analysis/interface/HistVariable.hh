#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include "CMSAnalysis/Analysis/interface/FilePathMapper.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include <array>
#include <string>

enum class Selector
{
    FirstHighestE,
    FirstHighestMu,
    E,
    Mu,
    None
};

enum class VariableType
{
    Eta,
    Pt,
    Phi,
    SameSignInvariantMass,
    OppositeSignInvariantMass,
    RecoInvariantMassBackground,
};

constexpr std::array<Selector, 5> selectors{
    {Selector::FirstHighestE, Selector::FirstHighestMu, Selector::E, Selector::Mu, Selector::None}};

constexpr std::array<VariableType, 6> variableTypes{
    {VariableType::Eta, VariableType::Pt, VariableType::Phi, VariableType::SameSignInvariantMass,
     VariableType::OppositeSignInvariantMass, VariableType::RecoInvariantMassBackground}};

class HistVariable
{
  public:
    HistVariable(Selector selector, VariableType var, bool is2DHistX = false, bool is2DHistY = false);

    std::string getName() const;

    bool is2DHistX()
    {
        return is2DHistX_;
    }
    bool is2DHistY()
    {
        return is2DHistY_;
    }

    void setSystematic(ScaleFactor::SystematicType itype, std::string isystematicName)
    {
        type = itype;
        systematicName = isystematicName;
    }
    ScaleFactor::SystematicType getSystematicType() const
    {
        return type;
    }
    std::string getSystematicName() const
    {
        return systematicName;
    }

  private:
    Selector selector;
    VariableType variableType;
    bool is2DHistX_ = false;
    bool is2DHistY_ = false;
    ScaleFactor::SystematicType type = ScaleFactor::SystematicType::Nominal;
    std::string systematicName;
};

#endif
