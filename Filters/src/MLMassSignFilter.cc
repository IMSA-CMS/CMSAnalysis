#include "CMSAnalysis/Filters/interface/MLMassSignFilter.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/EventBasic.hh"

std::string MLMassSignFilter::getFilterString(const EventInput* input) const
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

    EventBasic lj;

	if (highMass && sameSign) 
	{
        lj.setSignatureType(EventBasic::HIGH, EventBasic::SAME);
	} else if (highMass) 
	{
        lj.setSignatureType(EventBasic::HIGH, EventBasic::OPPOSITE);
	} else if (sameSign) 
	{
        lj.setSignatureType(EventBasic::LOW, EventBasic::SAME);
	} else {
    	lj.setSignatureType(EventBasic::LOW, EventBasic::OPPOSITE);
	}

	if (lj.isEqual(targetSig))
    {
        return lj.getSignatureType();
    } else {
        return "";
    }

	// if (highMass && sameSign && positive) 
	// {
	// 	return "High Mass and All Positive";
	// }

	// if (highMass && sameSign) 
	// {
	// 	return "High Mass and All Negative";
	// }

	// if (sameSign && positive) 
	// {
	// 	return "Low Mass and All Positive";
	// }

	// if (sameSign) 
	// {
	// 	return "Low Mass and All Negative";
	// }

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
