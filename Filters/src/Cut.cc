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

bool Cut::checkEvent(const Event& event, const EventInput* input, bool dependent)
{
    
    bool check = checkEventInternal(event, input);
    
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