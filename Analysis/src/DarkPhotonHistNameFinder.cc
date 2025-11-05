#include "CMSAnalysis/Analysis/interface/DarkPhotonHistNameFinder.hh"

std::string DarkPhotonHistNameFinder::getHistName(HistVariable histVariable) const
{
	std::string path = "hists/" + channel + "/" + subChannel + "/";
	// if (isData)
	// {
	// 	path += "_Pass";
	// }
	path += histVariable.getName();
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