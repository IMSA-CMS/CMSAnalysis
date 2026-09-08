#include "CMSAnalysis/Modules/interface/SystematicEventInput.hh"

#include <cmath>

// Constructor, just forwards eventInterface to the base class and stores variation
SystematicEventInput::SystematicEventInput(const EventInterface **eventInterface,
										   ScaleFactor::SystematicType variation2)
	: AnalyzerEventInput(eventInterface),
	  variation(variation2)

	  // Return a new Particle with pT multiplied by some "scale"
	  Particle SystematicEventInput::modifyParticlePt(const Particle &p, double dEscale) const
{
	auto vec = p.getFourVector();

	double oldPt = vec.Pt();
	double eta = vec.Eta();
	double phi = vec.Phi();

	// CMS formula: ΔpT = ΔE / cosh(η)
	double deltaPt = dEscale / std::cosh(eta);

	// CMS formula: pT_new = pT_old - ΔpT
	double newPt = oldPt - deltaPt;

	// Convert new pT back to px, py using φ
	double newPx = newPt * std::cos(phi);
	double newPy = newPt * std::sin(phi);

	// Build new Lorentz vector
	reco::Candidate::LorentzVector newVec(newPx, newPy, vec.Pz(), vec.E());

	return Particle(newVec, p.getCharge(), p.getType(), p.getSelectionFit());
}

// Overrides base getParticles. Call base, then apply SystematicEventInput scaling
ParticleCollection<Particle> SystematicEventInput::getParticles(
	RecoLevel level,
	const ParticleType &particleType,
	bool includeSpecials) const
{
	// Get the original particles from the base class
	ParticleCollection<Particle> baseParticles =
		AnalyzerEventInput::getParticles(level, particleType, includeSpecials);

	// Decide the scale factor based on variation (GET SCALE FACTORS, NEEDS TO BE DONE STILL I THINK)
	double scale = getScale();

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