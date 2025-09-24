#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <cassert>
#include <string>

static std::string defaultUnit(HistVariable::VariableType var)
{
    switch (var)
    {
    case HistVariable::VariableType::Pt:
        return "GeV";
    case HistVariable::VariableType::Eta:
        return "";
    case HistVariable::VariableType::Phi:
        return "";
    case HistVariable::VariableType::SameSignInvariantMass:
		return "GeV";
    case HistVariable::VariableType::OppositeSignInvariantMass:
		return "GeV";
    case HistVariable::VariableType::InvariantMass:
        return "GeV";
    default:
        return "";
    }
}

HistVariable::HistVariable(ParticleType type, int order, VariableType var, std::string unit, 
	bool is2DHistX, bool is2DHistY)
    : particleType(type),
      order_(order),
      variableType(var),
      unit(unit.empty() ? defaultUnit(var) : unit),
      is2DHistX_(is2DHistX),
      is2DHistY_(is2DHistY)
{
    assert(!(is2DHistX_ && is2DHistY_));
}

HistVariable::HistVariable(VariableType var, std::string unit, bool is2DHistX, bool is2DHistY) : 
    HistVariable(ParticleType::none(), 0, var, unit, is2DHistX, is2DHistY)
{}

std::string HistVariable::getName() const
{
    std::string name = "";

    if (order_ > 0)
    {
        switch (order_)
        {
        case 1:
            name += "1st ";
            break;
        case 2:
            name += "2nd ";
            break;
        case 3:
            name += "3rd ";
            break;
        default:
            name += std::to_string(order_) + "th ";
            break;
        }
        name += "Highest ";
    }

    if (particleType != ParticleType::none())
    {
        name += particleType.getName() + ' ';
    }


    switch (variableType)
    {
    case HistVariable::VariableType::Eta:
        name += "Eta";
        break;
    case HistVariable::VariableType::Pt:
        name += "Pt";
        break;
    case HistVariable::VariableType::Phi:
        name += "Phi";
        break;
    case HistVariable::VariableType::SameSignInvariantMass:
        name += "Same Sign Invariant Mass";
        break;
    case HistVariable::VariableType::OppositeSignInvariantMass:
        name += "Opposite Sign Invariant Mass";
        break;
    case HistVariable::VariableType::InvariantMass:
        name += "Reco Invariant Mass Background";
        break;
    }

    if (is2DHistX_)
    {
        name += " X Projection";
    }
    else if (is2DHistY_)
    {
        name += " Y Projection";
    }

    return name;
}

HistVariable::VariableType HistVariable::stringToVariableType(const std::string& varStr)
{
    if (varStr == "Eta") 
    {
        return VariableType::Eta;
    }
    if (varStr == "Phi") 
    {
        return VariableType::Phi;
    }
    if (varStr == "Pt") 
    {
        return VariableType::Pt;
    }
    if (varStr == "Same Sign Invariant Mass") 
    {
        return VariableType::SameSignInvariantMass;
    }
    if (varStr == "Opposite Sign Invariant Mass") 
    {
        return VariableType::OppositeSignInvariantMass;
    }
    if (varStr == "Reco Invariant Mass Background") 
    {
        return VariableType::InvariantMass;
    }

    throw std::invalid_argument("Unknown variable type: " + varStr);
}

int HistVariable::stringToOrder(const std::string& connecter)
{
    if (connecter.find("1st") != std::string::npos) 
    {
        return 1;
    }
    if (connecter.find("2nd") != std::string::npos)
    {
        return 2;
    } 
    if (connecter.find("3rd") != std::string::npos) 
    {
        return 3;
    }
    return 0;
}

ParticleType HistVariable::stringToParticle(const std::string& connecter)
{
    if (connecter.empty())
    {
        return ParticleType::none();
    }
    if (connecter.find("mu-") != std::string::npos) 
    {
        return ParticleType::muon();
    }
    if (connecter.find("e-")  != std::string::npos) 
    {
        return ParticleType::electron();
    }
    if (connecter.find("tau-")!= std::string::npos) 
    {
        return ParticleType::tau();
    }

    throw std::invalid_argument("Unknown particle type in connecter string: " + connecter);
}
