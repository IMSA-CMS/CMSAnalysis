#include "CMSAnalysis/DataCollection/interface/FourLeptonSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void FourLeptonSelector::selectParticles(const InputModule* input, Event& event)
{
    std::vector<Particle> selected;
    auto particles = input->getLeptons(InputModule::RecoLevel::Reco);
    
    
    if (particles.getNthHighestPt(1) > 0 && particles.getNthHighestPt(2) > 0 && particles.getNthHighestPt(3) > 0 && particles.getNthHighestPt(4) > 0)  
    {
        for(auto particle : particles) {
            if(particle.getType() == ParticleType::muon()) {
                event.addMuon(particle);
            }
            else if(particle.getType() == ParticleType::electron()) {
                event.addElectron(particle);
            }
        }
    }
}