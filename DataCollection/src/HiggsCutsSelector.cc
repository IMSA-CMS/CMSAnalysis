#include "CMSAnalysis/DataCollection/interface/HiggsCutsSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void HiggsCutsSelector::selectParticles(const InputModule* input, Event& event)
{
    std::cout << "Adding particles\n";
    auto particles = input->getLeptons(InputModule::RecoLevel::Reco);
    for(auto particle : particles) 
    {
        // if (particle.getPt() > 5)
        // {
            if(particle.getType() == ParticleType::muon()) 
            {
                // std::cout << "Adding muons of pt " << particle.getPt() << '\n';
                event.addMuon(particle);
            }
            else if(particle.getType() == ParticleType::electron()) 
            {
            event.addElectron(particle);
            // }
        }
    }
}