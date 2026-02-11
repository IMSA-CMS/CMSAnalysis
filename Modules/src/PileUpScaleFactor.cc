#include "CMSAnalysis/Modules/interface/PileUpScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <vector>
#include <string>
#include <limits>

PileUpScaleFactor::PileUpScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader)
    : EventScaleFactor(name, reader)
{
    
}

std::vector<std::string> PileUpScaleFactor::getKey(const EventInput* input) const
{
    int pileUp = input->getNumPileUpInteractions();
    std::string key;
    const auto& scaleFactorMap = getScaleFactorMap();
    for (const auto& entry : scaleFactorMap) 
    {
        int pileUpKey = std::stoi(entry.first);
        //std::cout<<(pileUpKey)<<'\n';
    

        if (pileUpKey > pileUp || pileUpKey == pileUp) 
        {
            key = entry.first;
            break;
        }
    }
    return {(key)};
}