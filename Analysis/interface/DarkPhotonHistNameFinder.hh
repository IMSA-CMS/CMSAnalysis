#ifndef DARKPHOTON_HIST_NAME_FINDER_HH
#define DARKPHOTON_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
class DarkPhotonHistNameFinder : public HistNameFinder
{
	public:
	DarkPhotonHistNameFinder(std::string iChannel, bool iisData = false) : channel(std::move(iChannel)), isData(iisData) {}
	virtual std::string getHistName(HistVariable histVariable) const override;

	private:
	std::string channel;
	bool isData;
};


#endif // DARKPHOTON_HIST_NAME_FINDER_HH