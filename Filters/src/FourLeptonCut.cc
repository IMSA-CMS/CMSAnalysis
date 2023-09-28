#include "CMSAnalysis/Filters/interface/FourLeptonCut.hh"

#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"

bool FourLeptonCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    if (event.getMuons().getParticles().size() + event.getElectrons().getParticles().size() < 4)
    {
        return false;
    }
    return true;
}