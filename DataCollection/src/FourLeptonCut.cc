#include "CMSAnalysis/DataCollection/interface/FourLeptonCut.hh"

#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"

bool FourLeptonCut::checkEventInternal(const Event& event) const
{
    if (event.getMuons().getParticles().size() + event.getElectrons().getParticles().size() < 4)
    {
        return false;
    }
    return true;
}