#ifndef MATCHEDLEPTONJETHIST_HH
#define MATCHEDLEPTONJETHIST_HH

#include "CMSAnalysis/DataCollection/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"

class LeptonJetMatchingModule;

class MatchedLeptonJetHist:public HistogramPrototype1D
{
public:
	MatchedLeptonJetHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetMatchingModule> ilepMatchMod, std::shared_ptr<LeptonJetReconstructionModule> ilepRecMod, bool matched=true);
	virtual std::vector<double> value() const;

private:
	std::shared_ptr<LeptonJetMatchingModule> lepMatchMod;
	std::shared_ptr<LeptonJetReconstructionModule> lepRecMod;
	bool matched;

};


#endif
