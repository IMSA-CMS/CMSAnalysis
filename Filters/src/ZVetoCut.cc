#include "CMSAnalysis/Filters/interface/ZVetoCut.hh"

#include <vector>
#include <unordered_map>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"

bool ZVetoCut::checkEventInternal(const Event& event) const
{
    const std::vector<Muon>& muons = event.getMuons().getParticles();
    const std::vector<Electron>& electrons = event.getElectrons().getParticles();

    double mass;
    for (int i = 0; i < int(electrons.size()) - 1; i++) 
    {
        for (int j = i+1; j < int(electrons.size()); j++)
        {
            mass = (electrons[i].getFourVector() + electrons[j].getFourVector()).M();
            if (electrons[i].getCharge() != electrons[j].getCharge() && (mass > 76 && mass < 106))
            {
                return false;
            }
        }
    }

    for (int i = 0; i < int(muons.size()) - 1; i++) 
    {
        for (int j = i+1; j < int(muons.size()); j++)
        {
            mass = (muons[i].getFourVector() + muons[j].getFourVector()).M();
            if (muons[i].getCharge() != muons[j].getCharge() && (mass > 76 && mass < 106))
            {
                
                return false;
            }
        }
    }
    return true;
}