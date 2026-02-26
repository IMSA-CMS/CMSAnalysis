#include "CMSAnalysis/Modules/interface/PileUpScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <vector>
#include <string>
#include <limits>

PileUpScaleFactor::PileUpScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader)
    : EventScaleFactor(name, reader)
{
    const auto& scaleFactorMap = getScaleFactorMap();
    for (const auto& entry : scaleFactorMap) 
    {
        int pileUpKey = std::stoi(entry.first);
        bins.insert(pileUpKey);
    }
}

std::vector<std::string> PileUpScaleFactor::getKey(const EventInput* input) const
{
    int pileUp = input->getNumPileUpInteractions();
    //std::cout << "Pile up: " << pileUp << std::endl;
    //std::cout << "Run #: " << input->getRunNum() << std::endl;
    int key = -1;
    for (const auto& entry : bins) 
    {
        if (entry > pileUp || entry == pileUp) 
        {
            key = entry;
            break;
        }
        key = entry;

    }
    if (key == -1)
    {
        throw std::runtime_error("Key not found in Pile Up Scale Factor");
    }
    return std::vector<std::string>{std::to_string(key)};
}