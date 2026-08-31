#ifndef EVENTSMEARINGUNCERTAINTY_HH
#define EVENTSMEARINGUNCERTAINTY_HH

#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"
#include "CMSAnalysis/Modules/interface/AnalyzerEventInput.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class SystematicEventInput : public AnalyzerEventInput
{
public:
	// Constructor: takes the EventInterface pointer and desired variation
	Uncertainty(const EventInterface **eventInterface,
				ScaleFactor::SystematicType variation);

	// Override getParticles to apply Up/Down/Nominal modifications
	virtual ParticleCollection<Particle> getParticles(
		RecoLevel level,
		const ParticleType &particleType = ParticleType::none(),
		bool includeSpecials = false) const override;

protected:
	virtual double getScale() const = 0;

private:
	// Which variation this instance should output
	UncertaintyVariation variation;

	// Helper function to modify a single particle's pT
	Particle modifyParticlePt(const Particle &p, double scale) const;
};

#endif