#include "CMSAnalysis/Modules/interface/LeptonJetTreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

std::vector<ParticleCollection<Particle>> LeptonJetTreeMakerModule::getParticles() const
{
    auto leptonJets = getInput()->getSpecial("leptonJet");

	return {leptonJets};
}