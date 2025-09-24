#include "CMSAnalysis/Modules/interface/MultiYearScaleFactorReader.hh"
#include <iostream>


void MultiYearScaleFactorReader::addYearData(const std::string& year, std::shared_ptr<ScaleFactorReader> reader)
{
    yearData[year] = reader;
}

std::map<std::string, ScaleFactor::ScaleFactorSet> MultiYearScaleFactorReader::readData()
{
    std::map<std::string, ScaleFactor::ScaleFactorSet> merged;
    for (const auto& yearEntry : yearData)
	{
		auto yearData = yearEntry.second->readData();
		for (const auto& dataEntry : yearData)
		{
			std::string key = yearEntry.first + "_" + dataEntry.first; // Create a unique key for each year and data entry
			merged[key] = dataEntry.second; // Merge the scale factors

			std::cout << "Added scale factor for year: " << yearEntry.first << ", data key: " << dataEntry.first << ", combined key: " << key << std::endl;
		}
	}
    return merged;
}