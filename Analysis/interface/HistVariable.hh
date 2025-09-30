#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include "CMSAnalysis/Analysis/interface/FilePathMapper.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include <array>
#include <string>



// constexpr std::array<Selector, 5> selectors{
//     {Selector::FirstHighestE, Selector::FirstHighestMu, Selector::E, Selector::Mu, Selector::None}};

// constexpr std::array<VariableType, 6> variableTypes{
//     {VariableType::Eta, VariableType::Pt, VariableType::Phi, VariableType::SameSignInvariantMass,
//      VariableType::OppositeSignInvariantMass, VariableType::RecoInvariantMassBackground}};

class HistVariable
{
  public:
    enum class VariableType
    {
        Eta,
        Pt,
        Phi,
        SameSignInvariantMass,
        OppositeSignInvariantMass,
        InvariantMass
    };

    HistVariable(ParticleType type, int order, VariableType var, std::string unit = "", 
        bool is2DHistX = false, bool is2DHistY = false);

    HistVariable(VariableType var, std::string unit = "", 
        bool is2DHistX = false, bool is2DHistY = false);

    std::string getName() const;

    ParticleType getParticleType() const
    {
        return particleType;
    }
   
    int getOrder() const
    {
        return order_;
    }

    VariableType getVariableType() const
    {
        return variableType;
    }

    std::string getUnit() const
    {
        return unit;
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

    static VariableType stringToVariableType(const std::string& varStr);
    static int stringToOrder(const std::string& connecter);
    static ParticleType stringToParticle(const std::string& connecter);

  private:
    ParticleType particleType;
    int order_;
    VariableType variableType;
    std::string unit;
    bool is2DHistX_ = false;
    bool is2DHistY_ = false;
    ScaleFactor::SystematicType type = ScaleFactor::SystematicType::Nominal;
    std::string systematicName;
};

#endif
