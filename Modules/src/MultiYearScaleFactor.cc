#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <stdexcept>

void MultiYearScaleFactor::addScaleFactor(std::string year, JSONScaleFactor scaleFactor)
{
	scaleFactors.insert({year, scaleFactor});
}
double MultiYearScaleFactor::getScaleFactor(const EventInput* input) const
{
	
	if (input->getFileParams()->getProcess() == "Data")
	{
		return 1.0;
	}

	auto year = input->getFileParams()->getParameters().find("Period");
	if (year == input->getFileParams()->getParameters().end())
	{
		throw std::runtime_error ("Year not found in file parameters");
	}
	auto sf = scaleFactors.find(year->second);
	if (sf == scaleFactors.end())
	{
		throw std::runtime_error ("Scale factors for year not found");
	}
	return sf->second.getScaleFactor(input);
}