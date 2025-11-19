#ifndef DARKPHOTON_HIST_NAME_FINDER_HH
#define DARKPHOTON_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
class DarkPhotonHistNameFinder : public HistNameFinder
{
	public:
	DarkPhotonHistNameFinder(std::string iChannel, std::string iSubchannel, bool iisData = false) 
	: channel(std::move(iChannel)), subChannel(std::move(iSubchannel)), isData(iisData) {}
	virtual std::string getHistName(const HistVariable& histVariable) const override;

	private:
	std::string channel;
	std::string subChannel;
	bool isData;
};


#endif // DARKPHOTON_HIST_NAME_FINDER_HH