#include "CMSAnalysis/Modules/interface/Uncertainty.hh"

#include <cmath>

// Constructor, just forwards eventInterface to the base class and stores variation
Uncertainty::Uncertainty(const EventInterface **eventInterface,
						 UncertaintyVariation variation_)
	: AnalyzerEventInput(eventInterface),
	  variation(variation_)

	  // Return a new Particle with pT multiplied by some "scale"
	  Particle Uncertainty::modifyParticlePt(const Particle &p, double scale) const
{
	auto vec = p.getFourVector();

	double oldPt = vec.Pt();
	double phi = vec.Phi();

	double newPt = oldPt * scale;
	double newPx = newPt * std::cos(phi);
	double newPy = newPt * std::sin(phi);

	reco::Candidate::LorentzVector newVec(newPx, newPy, vec.Pz(), vec.E());

	return Particle(newVec, p.getCharge(), p.getType(), p.getSelectionFit());
};

// Overrides base getParticles. Call base, then apply uncertainty scaling
ParticleCollection<Particle> Uncertainty::getParticles(
	RecoLevel level,
	const ParticleType &particleType,
	bool includeSpecials) const
{
	// Get the original particles from the base class
	ParticleCollection<Particle> baseParticles =
		AnalyzerEventInput::getParticles(level, particleType, includeSpecials);

	// Decide the scale factor based on variation (!!!INPUT LOGIC HERE FOR CHANGING PT!!!)
	double scale = 1.0;
	switch (variation)
	{
	case UncertaintyVariation::Up:
		scale = 1.05; // example up variation
		break;
	case UncertaintyVariation::Down:
		scale = 0.95; // example down variation
		break;
	case UncertaintyVariation::Nominal:
	default:
		scale = 1.0;
		break;
	}

	// If nominal, just return the base collection
	if (scale == 1.0)
		return baseParticles;

	// Build a new collection with modified particles
	ParticleCollection<Particle> out;

	for (const auto &p : baseParticles)
	{
		out.addParticle(modifyParticlePt(p, scale));
	}

	return out;
};