#ifndef LEPTONSCALEFACTORREADER_HH
#define LEPTONSCALEFACTORREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"

class LeptonScaleFactorReader : public ScaleFactorReader
{
public:
    LeptonScaleFactorReader(std::shared_ptr<ScaleFactorReader> electronReader, std::shared_ptr<ScaleFactorReader> muonReader);

	virtual std::map<std::string, ScaleFactor::ScaleFactorSet> readData() override;
	virtual std::string getKey(const EventInput* input) const override;
	std::string getKey(Lepton lepton) const;



private:
    std::shared_ptr<ScaleFactorReader> electronSF;
    std::shared_ptr<ScaleFactorReader> muonSF;
};

#endif
