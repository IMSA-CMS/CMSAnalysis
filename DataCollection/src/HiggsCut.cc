#include "CMSAnalysis/DataCollection/interface/HiggsCut.hh"

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

bool HiggsCut::checkEventInternal(const Event& event) const
{
    const auto particles = event.getParticles();
    //if(particles.calculateSameSignInvariantMass() < 800) {
    //    return false;
    //}
    if(particles.getLeadingPt() < 5) {
        return false;
    }

    return true;
}
