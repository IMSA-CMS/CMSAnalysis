#include "CMSAnalysis/Modules/interface/HEMScaleFactor.hh"

HEMScaleFactor::HEMScaleFactor(std::string iname) : ScaleFactor(std::move(iname), false)
{}

double HEMScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const
{

	return 1.0;
}