#include "CMSAnalysis/Modules/interface/MultiScaleFactor.hh"
double MultiScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const
{
	double totalScaleFactor = 1.0;
	for (const auto& scaleFactor : scaleFactors)
	{
		totalScaleFactor *= scaleFactor->getScaleFactor(input, type);
	}
	return totalScaleFactor;
}

std::string MultiScaleFactor::getKey(const EventInput* input) const
{
	// will not be called
}
