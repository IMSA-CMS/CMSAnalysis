#include "CMSAnalysis/Modules/interface/EventInput.hh"

ParticleCollection<Lepton> EventInput::getLeptons(RecoLevel level) const
{
    ParticleCollection<Lepton> leptons;
    auto electrons = getParticles(level, ParticleType::electron()).getParticles();
    auto muons = getParticles(level, ParticleType::muon()).getParticles();
    auto taus = getParticles(level, ParticleType::tau()).getParticles();
    //std::cout << "Electrons: " << electrons.size() << "\nMuons: " << muons.size() << "\n";
    for (const auto &p : electrons)
    {
        leptons.addParticle(p);
    }
    for (const auto &p : muons)
    {
        leptons.addParticle(p);
    }
    for (const auto &p : taus)
    {
        leptons.addParticle(p);
    }
    return leptons;
}