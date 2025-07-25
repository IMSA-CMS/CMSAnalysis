#include "CMSAnalysis/Analysis/interface/DarkPhotonHistNameFinder.hh"

std::string DarkPhotonHistNameFinder::getHistName(HistVariable histVariable) const
{
	std::string path = channel + "__hists/" + channel;
	if (isData)
	{
		path += "_Pass";
	}
	path += "_" + histVariable.getName();
// Add systematic type
	
	if (histVariable.getSystematicType() == ScaleFactor::SystematicType::Up)
	{
		return path + "_" + histVariable.getSystematicName() + "_Up";
	}
	else if (histVariable.getSystematicType() == ScaleFactor::SystematicType::Down)
	{
		return path + "_" + histVariable.getSystematicName() + "_Down";
	}
	else
	{
		return path;
	}
}