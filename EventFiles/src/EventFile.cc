#include "CMSAnalysis/EventFiles/interface/EventFile.hh"

double EventFile::getEventQuantity(std::string key) const
	{
		auto value = eventQuantities.find(key);
		if (value == eventQuantities.end())
		{
			throw std::runtime_error("Key not found in getEventQuantities");
		}
		return value->second;
	}

void EventFile::setEventQuantity(std::string key, double value)
	{
		eventQuantities.insert({key,value});
	}