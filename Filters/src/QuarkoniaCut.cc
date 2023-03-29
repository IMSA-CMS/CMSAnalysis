#include "CMSAnalysis/DataCollection/interface/QuarkoniaCut.hh"

#include <vector>
#include <unordered_map>
#include <cmath>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"

bool QuarkoniaCut::checkEventInternal(const Event& event) const
{
    const std::vector<Muon>& muons = event.getMuons().getParticles();
    const std::vector<Electron>& electrons = event.getElectrons().getParticles();

    double mass;
    for (int i = 0; i < int(electrons.size()) - 1; i++) 
    {
        for (int j = i+1; j < int(electrons.size()); j++)
        {
            mass = (electrons[i].getFourVector() + electrons[j].getFourVector()).M();
            if (electrons[i].getCharge() != electrons[j].getCharge() && mass < 12 )
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
            if (muons[i].getCharge() != muons[j].getCharge() && mass < 12)
            {
                return false;
            }
        }
    }
    return true;
}