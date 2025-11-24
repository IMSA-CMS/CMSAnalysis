#ifndef HIGGS_HIST_NAME_FINDER_HH
#define HIGGS_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
class HiggsHistNameFinder : public HistNameFinder
{
	public:
	HiggsHistNameFinder(std::string iChannel, bool iisData = false, bool izSelection = true) : channel(std::move(iChannel)), isData(iisData), zSelection(izSelection) {}
	virtual std::string getHistName(HistVariable histVariable) const override;

	private:
	std::string channel;
	bool isData;
	bool zSelection;
};


#endif // HIGGS_HIST_NAME_FINDER_HH