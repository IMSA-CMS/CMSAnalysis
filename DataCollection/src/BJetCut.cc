#include "CMSAnalysis/DataCollection/interface/BJetCut.hh"

#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"

bool BJetCut::checkEventInternal(const Event& event) const
{
    const std::vector<Jet>& jets = event.getJets().getParticles();
    for (int i = 0; i < int(jets.size()); i++) 
    {
        if (jets[i].getInfo("bJet"))
        {
            return false;
        }
    }
    return true;
}