#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>
#include <limits>

LeptonScaleFactor::LeptonScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader, ParticleType particleType) 
    : ScaleFactor(name, reader), particleType(particleType)
{
}

std::vector<std::string> LeptonScaleFactor::getKey(const EventInput* input) const
{
	// Implement your logic to retrieve the scale factor using the reader
	auto particles = input->getParticles(EventInput::RecoLevel::Reco, particleType);

	std::vector<std::string> keys;
	for (const auto& particle : particles) 
	{
		auto leptonKey = getKey(particle);
		keys.push_back(leptonKey);
	}
	return keys;

}

std::string LeptonScaleFactor::getKey(Lepton lepton) const
{
	
    // Go through scaleFactorMap
    // Find highest eta key less than your eta and lowest pT key more than your pT in the keys.

    //std::cout << "getKey in LeptonScaleFactor is called" << std::endl;

    std::string bestKey = "";
    double bestEta = -1;
    double bestPt = std::numeric_limits<double>::max();

    for (const auto& entry : getScaleFactorMap()) {
        //std::cout << "enters for loop";
        std::string key = entry.first;

        
        size_t underscorePos1 = key.find("_");
        size_t underscorePos2 = key.find("_", underscorePos1 + 1);
        double eta = std::stod(key.substr(underscorePos1 + 1, underscorePos2 - underscorePos1 - 1));
        double pt = std::stod(key.substr(underscorePos2 + 1));
        //std::cout << "eta: " << eta <<" lepton.getEta():"<<lepton.getEta()<< ", pt: " << pt << " lepton.getPt():" << lepton.getPt() << std::endl;

        if (eta < std::abs(lepton.getEta()) && pt > lepton.getPt())
        {
        //std::cout << "first condition met" << std::endl;

            if (eta > bestEta || (eta == bestEta && pt < bestPt)) {
           // std::cout << "second condition met" << std::endl;
            
                bestKey = key;
                bestEta = eta;
                bestPt = pt;
            }
        }
    }
    // Remove year from bestKey
    size_t underscorePos = bestKey.find("_");
    if (underscorePos != std::string::npos) 
    {
        bestKey = bestKey.substr(underscorePos + 1);
    }
   // std::cout << "Best key found: " << bestKey << std::endl;
    //std::cout << "----------------------------------\n-------------------------\n" << std::endl;

    return bestKey;
}   
