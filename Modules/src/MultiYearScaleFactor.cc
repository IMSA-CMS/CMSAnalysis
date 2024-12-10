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
	auto sf = scaleFactors.find(input->getFileParams()->getParameters().at("Period"));
	
	if (input->getFileParams()->getProcess() == "Data")
	{
		std::cout << "Process is Data, returning scale factor 1.0" << std::endl;
		return 1.0;
	}
	auto year = input->getFileParams()->getParameters().find("Period");
	if (year == input->getFileParams()->getParameters().end())
	{
		throw std::runtime_error ("Year not found in file parameters");
	}
	if (sf == scaleFactors.end())
	{
		throw std::runtime_error ("Scale factors for year not found");
	}	
	// auto answer = sf->second.getScaleFactor(input);
	// std::cout << "Scale factor: " << answer << std::endl;	
	// return answer;
	return sf->second.getScaleFactor(input);
}