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

HistVariable::HistVariable(VariableType var, std::string unit, bool is2DHistX, bool is2DHistY) : HistVariable(ParticleType::none(), 0, var, unit, is2DHistX, is2DHistY)
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
    }

    if (particleType != ParticleType::none())
    {
        if (particleType == ParticleType::electron())      
        {
            name += "e- ";
        }
        else if (particleType == ParticleType::muon()) 
        {
            name += "mu- ";
        }    
        else if (particleType == ParticleType::tau()) 
        {
            name += "tau- ";
        }
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

	if (!unit.empty())
	{
	   // name += " [" + unit + "]";
	}

    return name;
}
