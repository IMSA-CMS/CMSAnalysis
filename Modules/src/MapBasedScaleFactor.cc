#include "CMSAnalysis/Modules/interface/MapBasedScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <stdexcept>


MapBasedScaleFactor::MapBasedScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader):
ScaleFactor(iname),
reader(ireader),
scaleFactors(reader->readData())
{

}

double MapBasedScaleFactor::getScaleFactor(const EventInput* input, SystematicType type ) const
{
	if (input->getFileParams()->getProcess() == "Data")
 	{
	 	return 1.0; // No scale factor for data
 	}
	auto keys = getKey(input);
	double scaleFactor = 1; 
	for (auto key : keys)
	{
		auto entry = scaleFactors.find(key);
		if (entry == scaleFactors.end())
		{
			throw std::runtime_error("Scale factor " + key + " not found!");
		}
		//std::cout << "Scale factor " << name << ": " << " key " << key << " sys type " << static_cast<int>(type) << " value " << entry->second.getSystematic(type) << std::endl;
		scaleFactor *= entry->second.getSystematic(type);
	}
	return scaleFactor;
}
