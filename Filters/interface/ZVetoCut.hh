#ifndef ZVETOCUT_HH
#define ZVETOCUT_HH
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

class ZVetoCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event) const override;
};

#endif