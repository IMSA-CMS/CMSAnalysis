#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"

std::string HiggsHistNameFinder::getHistName(HistVariable histVariable, ScaleFactor::SystematicType type, std::string systematicName) const
{
	std::string path = channel + "__hists/" + channel;
	if (isData)
	{
		path += "_Pass";
	}
	path += "_" + histVariable.getName();
// Add systematic type
	
	if (type == ScaleFactor::SystematicType::Up)
	{
		return path + "_" + systematicName + "_Up";
	}
	else if (type == ScaleFactor::SystematicType::Down)
	{
		return path + "_" + systematicName + "_Down";
	}
	else
	{
		return path;
	}
}