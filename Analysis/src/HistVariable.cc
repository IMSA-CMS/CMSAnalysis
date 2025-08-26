#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <cassert>
#include <string>

static std::string defaultUnit(VariableType var)
{
    switch (var)
    {
    case VariableType::Pt:
        return "GeV";
    case VariableType::Eta:
        return "";
    case VariableType::Phi:
        return "rad";
    case VariableType::SameSignInvariantMass:
		return "GeV";
    case VariableType::OppositeSignInvariantMass:
		return "GeV";
    case VariableType::RecoInvariantMassBackground:
        return "GeV";
    default:
        return "";
    }
}

HistVariable::HistVariable(Selector selector, VariableType var, std::string unit, 
	bool is2DHistX, bool is2DHistY)
    : selector(selector), variableType(var), unit(unit.empty() ? defaultUnit(var) : unit),
	is2DHistX_(is2DHistX), is2DHistY_(is2DHistY)
{
    assert(!(is2DHistX_ && is2DHistY_));
}

std::string HistVariable::getName() const
{
    std::string name = "";

    switch (selector)
    {
    case Selector::FirstHighestMu:
        name += "1st Highest mu- ";
        break;
    case Selector::FirstHighestE:
        name += "1st Highest e- ";
        break;
    case Selector::E:
        name += "e- ";
        break;
    case Selector::Mu:
        name += "mu- ";
        break;
    case Selector::None:
        break;
    }

    switch (variableType)
    {
    case VariableType::Eta:
        name += "Eta";
        break;
    case VariableType::Pt:
        name += "Pt";
        break;
    case VariableType::Phi:
        name += "Phi";
        break;
    case VariableType::SameSignInvariantMass:
        name += "Same Sign Invariant Mass";
        break;
    case VariableType::OppositeSignInvariantMass:
        name += "Opposite Sign Invariant Mass";
        break;
    case VariableType::RecoInvariantMassBackground:
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
	    name += " [" + unit + "]";
	}

    return name;
}
