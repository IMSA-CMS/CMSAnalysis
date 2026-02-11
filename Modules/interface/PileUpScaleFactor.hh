#ifndef PILEUPSCALEFACTOR_HH
#define PILEUPSCALEFACTOR_HH
#include "CMSAnalysis/Modules/interface/EventScaleFactor.hh"
#include <set>

class PileUpScaleFactor : public EventScaleFactor
{
	public:
		PileUpScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader);

	protected:
		virtual std::vector<std::string> getKey(const EventInput* input) const override;
	
	private:
		std::set<double> bins;
};

#endif 