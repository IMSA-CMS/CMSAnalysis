#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include <stdexcept>

ScaleFactor::ScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader):
name(iname),
scaleFactors(ireader->readData())
{

}

double ScaleFactor::getScaleFactor(const EventInput* input, SystematicType type ) const
{
	auto key = getKey(input);
	auto entry = scaleFactors.find(key);
	if (entry == scaleFactors.end())
	{
		throw std::runtime_error("Scale factor not found!");
	}
			return entry->second.getSystematic(type);

}
