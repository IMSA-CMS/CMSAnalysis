#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"

std::string HiggsHistNameFinder::getHistName(HistVariable histVariable, SystematicType type) const
{
	std::string path = channel + "__hists/" + channel + "_" + histVariable.getName();
	return path;
}
