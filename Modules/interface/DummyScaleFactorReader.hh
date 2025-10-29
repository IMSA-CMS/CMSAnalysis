#ifndef DUMMYSCALEFACTORREADER_HH
#define DUMMYSCALEFACTORREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"

class DummyScaleFactorReader : public ScaleFactorReader
{
public:

    DummyScaleFactorReader() {}
    // virtual double getScaleFactor(const EventInput* input) const override;
    virtual std::map<std::string, ScaleFactor::ScaleFactorSet> readData() override
	{
		return {};
	}

};

#endif // DUMMYSCALEFACTORREADER_HH
