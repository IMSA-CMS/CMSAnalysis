#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <stdexcept>


ScaleFactor::ScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader):
name(iname),
reader(ireader),
scaleFactors(reader->readData())
{

}

double ScaleFactor::getScaleFactor(const EventInput* input, SystematicType type ) const
{
	if (input->getFileParams()->getProcess() == "Data")
 	{
	 return 1.0; // No scale factor for data
 	}
	auto key = reader->getKey(input);
	auto entry = scaleFactors.find(key);
	if (entry == scaleFactors.end())
	{
		throw std::runtime_error("Scale factor not found!");
	}
			return entry->second.getSystematic(type);

}
