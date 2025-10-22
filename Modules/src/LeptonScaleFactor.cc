#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>
#include <limits>
#include "CMSAnalysis/Utility/interface/FileParams.hh"

LeptonScaleFactor::LeptonScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader, ParticleType particleType) 
    : ScaleFactor(name, reader), particleType(particleType)
{
	// Extract eta and pt bins from the scale factor map
	for (const auto& entry : getScaleFactorMap()) 
	{
		std::string key = entry.first;
		size_t underscorePos = key.find("_");
		if (underscorePos != std::string::npos) 
		{
			double eta = std::stod(key.substr(0, underscorePos));
			double pt = std::stod(key.substr(underscorePos + 1));
			bins[eta].insert(pt);
		}
	}
}

std::vector<std::string> LeptonScaleFactor::getKey(const EventInput* input) const
{
	// Implement your logic to retrieve the scale factor using the reader
	auto particles = input->getParticles(EventInput::RecoLevel::Reco, particleType);

    // auto year = input->getFileParams()->getParameters().find("Year");
    // if (year == input->getFileParams()->getParameters().end())
    // {
    //     throw std::runtime_error ("Year not found in file parameters");
    // }

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

	// Check etaBins and ptBins for the best match
	for (auto it = bins.rbegin(); it != bins.rend(); ++it) 
	{
		double eta = it->first;
		if (eta < std::abs(lepton.getEta())) 
		{
			bestEta = eta;
			for (const auto& pt : it->second) 
			{
				if (pt > lepton.getPt()) 
				{
					bestPt = pt;
					break;
				}
			}
			break; // Found the best eta, no need to continue
		}
	}




    // for (const auto& entry : getScaleFactorMap()) {
    //     //std::cout << "enters for loop";
    //     std::string key = entry.first;

    //     // if (key.substr(0, year.size()) != year)
    //     // {
    //     //     continue; // Skip keys that don't match the year
    //     // }
    //     size_t underscorePos1 = key.find("_");
    //     // size_t underscorePos2 = key.find("_", underscorePos1 + 1);
    //     double eta = std::stod(key.substr(0,underscorePos1));
    //     double pt = std::stod(key.substr(underscorePos1 + 1));
    //     //std::cout << "eta: " << eta <<" lepton.getEta():"<<lepton.getEta()<< ", pt: " << pt << " lepton.getPt():" << lepton.getPt() << std::endl;

    //     if (eta < std::abs(lepton.getEta()) && pt > lepton.getPt())
    //     {
    //     //std::cout << "first condition met" << std::endl;

    //         if (eta > bestEta || (eta == bestEta && pt < bestPt)) {
    //         //std::cout << "second condition met" << std::endl;
            
    //             bestKey = key;
    //             bestEta = eta;
    //             bestPt = pt;
    //         }
    //     }
    // }
    // Remove year from bestKey
    //std::cout << "Best key (first time):" << bestKey << std::endl;
    // size_t underscorePos = bestKey.find("_");
    // //std::cout << "underscore pos:" << underscorePos << std::endl;
    // if (underscorePos != std::string::npos) 
    // {
    //     //std::cout << "underscorePos WAS found " << std::endl;
    //     //std::cout << "Best key:" << bestKey << std::endl;
    //     bestKey = bestKey.substr(underscorePos + 1);
    // }
    //std::cout << "Best key found: " << bestKey << std::endl;
    //std::cout << "----------------------------------\n-------------------------\n" << std::endl;

	// std::cout << "Best eta: " << bestEta << ", Best pt: " << bestPt << std::endl;
	// std::cout << "Final key: " << std::to_string(bestEta) + "_" + std::to_string(bestPt) << std::endl;
    return std::to_string(bestEta) + "_" + std::to_string(bestPt);
}   
