#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/DummyScaleFactorReader.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <stdexcept>
#include <iostream>
#include <memory>


MultiYearScaleFactor::MultiYearScaleFactor(std::string name)
	: ScaleFactor(name, std::make_shared<DummyScaleFactorReader>()) // Using a dummy reader since we don't read data directly here
{
}

void MultiYearScaleFactor::addScaleFactor(std::string year, std::shared_ptr<ScaleFactor> scaleFactor) 
{
	scaleFactors[year] = scaleFactor;
}

double MultiYearScaleFactor::getScaleFactor(const EventInput* input, ScaleFactor::SystematicType type) const
{
	auto yearIt = input->getFileParams()->getParameters().find("Year");
	if (yearIt == input->getFileParams()->getParameters().end())
	{
		throw std::runtime_error("Year not found in file parameters");
	}
	
	const std::string& year = yearIt->second;
	auto sfIt = scaleFactors.find(year);
	if (sfIt == scaleFactors.end())
	{
		throw std::runtime_error("No scale factor found for year: " + year);
	}
	
	return sfIt->second->getScaleFactor(input, type);
}


std::vector<std::string> MultiYearScaleFactor::getKey(const EventInput* input) const
{
	throw std::runtime_error("getKey method is not implemented for MultiYearScaleFactor");
}