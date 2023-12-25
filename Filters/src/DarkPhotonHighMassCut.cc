#include "CMSAnalysis/Filters/interface/DarkPhotonHighMassCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

bool DarkPhotonHighMassCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	const auto leptonJets = event.getSpecial("leptonJet");

	for (auto leptonJet : leptonJets) 
	{
		if (leptonJet.getMass() > minJetMass) 
		{
			return true;
		}
	}

	return false;
}
