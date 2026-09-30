#include <exception>
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"

double ParticleImplementation::getInfo(std::string mapKey) const
{
	auto entry = infoMap.find(mapKey);
	if (entry == infoMap.end())
	{
		throw std::runtime_error("Key " + mapKey + " was not found");
	}
	else
	{
		return entry->second;
	}
}

void ParticleImplementation::addInfo(std::string mapKey, double value) 
{
	// If it does not exist, will create new entry. If it does exist, will replace value
	infoMap[mapKey] = value;
}

