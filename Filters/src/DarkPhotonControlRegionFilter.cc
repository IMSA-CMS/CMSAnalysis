#include "CMSAnalysis/Filters/interface/DarkPhotonControlRegionFilter.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

std::string DarkPhotonControlRegionFilter::getFilterString(const EventInput* input) const
{
	const auto leptonJets = input->getSpecial("leptonJet");

	if (leptonJets.size() == 0) 
	{
		return "";
	}

	auto leptonJet = LeptonJet(leptonJets[0]);

	// if ((leptonJet.getEta() > 1 && leptonJet.getEta() < 1.5) || (leptonJet.getEta() > -1.5 && leptonJet.getEta() < -1)) 
	// {
	// 	return "Bad Eta";
	// }

	bool highMass = leptonJet.getMass() > minJetMass;
	//bool highDXYDZ = leptonJet.getMaxDXY() > minDXY && leptonJet.getMaxDZ() > minDZ;
	bool sameSign = leptonJet.allSameSign();
	//bool positive = leptonJet.getTotalCharge() > 0;

	if (highMass && sameSign) 
	{
		return "High Mass and Same Sign";
	}

	// if (highMass && sameSign && positive) 
	// {
	// 	return "High Mass and All Positive";
	// }

	// if (highMass && sameSign) 
	// {
	// 	return "High Mass and All Negative";
	// }

	if (highMass) 
	{
		return "High Mass and Different Sign";
	}

	if (sameSign) 
	{
		return "Low Mass and Same Sign";
	}

	// if (sameSign && positive) 
	// {
	// 	return "Low Mass and All Positive";
	// }

	// if (sameSign) 
	// {
	// 	return "Low Mass and All Negative";
	// }

	return "Low Mass and Different Sign";

	// if (highMass && highDXYDZ) 
	// {
	// 	return "High Mass and High DXY DZ";
	// }

	// if (highMass) 
	// {
	// 	return "High Mass and Low DXY DZ";
	// }

	// if (highDXYDZ) 
	// {
	// 	return "Low Mass and High DXY DZ";
	// }

	// return "Low Mass and Low DXY DZ";
}
