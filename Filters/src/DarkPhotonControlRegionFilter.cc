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

	bool highMass = leptonJet.getMass() > minJetMass;
	bool highDXYDZ = leptonJet.getMaxDXY() > minDXY && leptonJet.getMaxDZ() > minDZ;

	if (highMass && highDXYDZ) 
	{
		return "High Mass and High DXY DZ";
	}

	if (highMass) 
	{
		return "High Mass and Low DXY DZ";
	}

	if (highDXYDZ) 
	{
		return "Low Mass and High DXY DZ";
	}

	return "Low Mass and Low DXY DZ";
}
