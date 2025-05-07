#ifndef HIGGS_HIST_NAME_FINDER_HH
#define HIGGS_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"

class HiggsHistNameFinder : public HistNameFinder
{
	public:
	HiggsHistNameFinder(std::string iChannel) : channel(std::move(iChannel)) {}
	virtual std::string getHistName(HistVariable histVariable) const override;

	private:
	std::string channel;
};


#endif // HIGGS_HIST_NAME_FINDER_HH