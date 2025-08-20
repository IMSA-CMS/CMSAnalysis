#include "CMSAnalysis/Modules/interface/MultiYearScaleFactorReader.hh"

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
			std::string key = yearEntry.first + dataEntry.first; // Create a unique key for each year and data entry
			merged[key] = dataEntry.second; // Merge the scale factors
		}
	}
    return merged;
}