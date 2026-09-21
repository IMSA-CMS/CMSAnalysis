#ifndef HEMSCALEFACTOR_HH
#define HEMSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class HEMScaleFactor : public ScaleFactor
{
public:
	HEMScaleFactor(std::string iname);

	virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;
};
#endif