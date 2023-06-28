#include "CMSAnalysis/Filters/interface/MismeasuredChargeFilter.hh"

MismeasuredChargeFilter::MismeasuredChargeFilter()
{}

std::string MismeasuredChargeFilter::getFilterString(const InputModule* inputMod) const
{
	auto recoParts = inputMod->getParticles(InputModule::RecoLevel::Reco).getParticles();
	auto genParts = inputMod->getParticles(InputModule::RecoLevel::GenSim).getParticles();
	bool MismeasuredCharge = false;
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
					MismeasuredCharge = false;
					break;
				}
				if (genPart.getCharge() == -recoPart.getCharge())
				{
					MismeasuredCharge = true;
				}
			}
		}
		if (MismeasuredCharge)
		{
			return "MismeasuredCharge";
		}
	}
	return "real";
};