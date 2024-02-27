#include "CMSAnalysis/Filters/interface/DarkPhotonHighMassFilter.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

std::string DarkPhotonHighMassFilter::getFilterString(const EventInput* input) const
{
	const auto leptonJets = input->getSpecial("leptonJet");

	for (auto leptonJet : leptonJets) 
	{
		if (leptonJet.getMass() > minJetMass) 
		{
			return "High Mass";
		}
	}

	return "Signal Region";
}
