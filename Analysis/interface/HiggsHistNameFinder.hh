#ifndef HIGGS_HIST_NAME_FINDER_HH
#define HIGGS_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
class HiggsHistNameFinder : public HistNameFinder
{
	public:
	HiggsHistNameFinder(std::string iChannel, bool iisData = false) : channel(std::move(iChannel)), isData(iisData) {}
	virtual std::string getHistName(HistVariable histVariable, ScaleFactor::SystematicType type, std::string systematicName) const override;

	private:
	std::string channel;
	bool isData;
};


#endif // HIGGS_HIST_NAME_FINDER_HH