#include "CMSAnalysis/DataCollection/interface/DarkPhotonGenSimSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

std::vector<Particle> DarkPhotonGenSimSelector::selectParticles(const InputModule* input) const
{
    std::vector<Particle> selected(0);
    auto particles = input->getParticles(InputModule::RecoLevel::GenSim);

    for (const auto& particle : particles)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);
        if (genSimParticle.pdgId() == 4900022) //Dark Photon
        {
            genSimParticle = genSimParticle.finalDaughter();
            if (genSimParticle.numberOfDaughters() == 2)
            {
                GenSimParticle daughter1 = genSimParticle.daughter(0);
                GenSimParticle daughter2 = genSimParticle.daughter(1);
                selected.push_back(genSimParticle);
                selected.push_back(daughter1);
                selected.push_back(daughter2);
            }
        }
        if (abs(genSimParticle.pdgId() == 1000022)) //neutralino
        {
            genSimParticle = genSimParticle.finalDaughter();
            if (genSimParticle.numberOfDaughters() == 2)
            {
                GenSimParticle daughter1 = genSimParticle.daughter(0);
                GenSimParticle daughter2 = genSimParticle.daughter(1);
                selected.push_back(genSimParticle);
                selected.push_back(daughter1);
                selected.push_back(daughter2);
            }
        }
    }
    return selected;
}