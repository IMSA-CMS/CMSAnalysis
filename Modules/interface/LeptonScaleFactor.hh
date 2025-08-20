#ifndef LEPTONSCALEFACTOR_HH
#define LEPTONSCALEFACTOR_HH
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class LeptonScaleFactor : public ScaleFactor
{

	public:
	LeptonScaleFactor(std::shared_ptr<LeptonScaleFactorReader> reader);
	virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;

};
#endif