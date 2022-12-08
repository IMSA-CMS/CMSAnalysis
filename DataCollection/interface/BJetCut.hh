#ifndef BLEPTONCUT_HH
#define BLEPTONCUT_HH
#include <vector>
#include <unordered_map>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"

class BJetCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event) const override;
};

#endif