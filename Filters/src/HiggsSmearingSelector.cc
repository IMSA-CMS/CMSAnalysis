#include 	"CMSAnalysis/Filters/interface/HiggsSmearingSelector.hh"

void HiggsSmearingSelector::adjustEnergy(Particle& particle) const
{
	if (particle.getType() == ParticleType::electron())
	{
		double correction = 0;
		if (type == ScaleFactor::SystematicType::Down)
		{
			correction = particle.getInfo("SmearingDown");
		}
		else if (type == ScaleFactor::SystematicType::Up)
		{
			correction = particle.getInfo("SmearingUp");
		}
		
		double newPt = particle.getPt() - correction / std::cosh(particle.getEta());
	}
}

