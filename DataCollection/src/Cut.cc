#include "CMSAnalysis/DataCollection/interface/ZVetoCut.hh"

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

bool Cut::checkEvent(const Event& event, bool dependent)
{
    
    bool check = checkEventInternal(event);
    
    if (dependent)
    {
        checkCount++;
        if(check)
        {
            passCount++;
        }
    }
    else 
    {
        independentCheckCount++;
        if(check)
        {
            independentPassCount++;
        }
    }
    return check;
}