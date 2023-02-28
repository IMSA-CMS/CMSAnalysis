#include "CMSAnalysis/DataCollection/interface/FakePhotonFilter.hh"

FakePhotonFilter::FakePhotonFilter()
{}

std::string FakePhotonFilter::makeFilterString()
{
	auto recoParts = getInput()->getParticles(InputModule::RecoLevel::Reco).getParticles();
	auto genParts = getInput()->getParticles(InputModule::RecoLevel::GenSim).getParticles();
	bool fakePhoton = false;
	for (auto &recoPart : recoParts)
	{
		for (auto &genPart : genParts)
		{
			double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
			if (deltaR < 0.1)
			{
				if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge())
				{
					// accurateMatchCounter++;
					fakePhoton = false;
					break;
				}
				if (genPart.getType() == ParticleType::photon())
				{
					fakePhoton = true;
				}
			}
		}
		if (fakePhoton)
		{
			return "FakePhoton";
		}
	}
	return "real";
};
