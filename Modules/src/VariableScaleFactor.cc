#include "CMSAnalysis/Modules/interface/VariableScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

VariableScaleFactor::VariableScaleFactor(std::string iname, std::string variableNameNominal, std::string variableNameUp, std::string variableNameDown) :
 ScaleFactor(iname),
 variableNameNominal(variableNameNominal),
 variableNameUp(variableNameUp),
 variableNameDown(variableNameDown)
{
}

double VariableScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const
{
	switch (type)
	{
		case SystematicType::Nominal:
			return input->getEventQuantity(variableNameNominal);
		case SystematicType::Up:
			return input->getEventQuantity(variableNameUp);
		case SystematicType::Down:
			return input->getEventQuantity(variableNameDown);
		default:
			throw std::runtime_error("Invalid systematic type");
	}

}