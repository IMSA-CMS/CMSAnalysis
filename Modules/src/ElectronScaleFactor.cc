#include "CMSAnalysis/Modules/interface/ElectronScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>


ElectronScaleFactor::ElectronScaleFactor(std::string filename) : JSONScaleFactor(filename)
{
	loadScaleFactorsFromFile(filename);
}

ParticleCollection<Particle> ElectronScaleFactor::getParticles(const EventInput* input) const
{
    return input->getParticles(EventInput::RecoLevel::Reco, ParticleType::electron());
}

void ElectronScaleFactor::loadScaleFactors(Json::Value output)
{
    // Load nominal scale factors
    loadScaleFactors(output, SystematicType::Nominal);
    // Load up systematic scale factors
    loadScaleFactors(output, SystematicType::Up);
    // Load down systematic scale factors
    loadScaleFactors(output, SystematicType::Down);
}

void ElectronScaleFactor::loadScaleFactors(Json::Value output, SystematicType systematicType)
{
    
    Json::Value allCorrections = output["corrections"];
    Json::Value allData = allCorrections[0u]["data"];
    
    Json::Value allEdges = allData["edges"];
    //std::cout << "No data found" << std::endl;
    Json::Value content = allData["content"];
    //std::cout << "No content found" << std::endl;

    // Loop through the years
    for (size_t i = 0; i < content.size(); i++) 
    {
        Json::Value yearEntry = content[i];
        Json::Value yearKey = yearEntry["key"];

        //std::cout << "No year key found" << std::endl;
        //std::string year = yearKey.asString();
        //std::cout << "Processing year: " << year << std::endl;

        Json::Value yearValue = yearEntry["value"];
        Json::Value yearContent = yearValue["content"];
        //std::cout << "No year content found" << std::endl;

        // Loop through ValType (e.g., "sf")
        for (size_t j = 0; j < yearContent.size(); j++)
        {
            switch (systematicType)
            {
                case SystematicType::Nominal:
                    if (yearContent[j]["key"] != "sf")
                    {
                        continue;
                    }
                    break;
                case SystematicType::Up:
                    if (yearContent[j]["key"] != "sfup")
                    {
                        continue;
                    }
                    break;
                case SystematicType::Down:
                    if (yearContent[j]["key"] != "sfdown")
                    {
                        continue;
                    }
                    break;
            }
            
            Json::Value valTypeEntry = yearContent[j]["value"];

            //std::cout << "No ValType key found" << std::endl;
            

            Json::Value workingPointContent = valTypeEntry["content"];
            //std::cout << "No working point content found" << std::endl;

            // Loop through WorkingPoints (e.g., "RecoBelow20" and "RecoAbove20")
            for (size_t k = 0; k < workingPointContent.size(); k++)
            {
                if (workingPointContent[k]["key"] != "Loose")
                {
                    continue;
                }
                Json::Value wpEntry = workingPointContent[k];
                //std::cout << "No WorkingPoint key found" << std::endl;
                //std::string workingPoint = wpKey.asString();
                //std::cout << "Processing WorkingPoint: " << workingPoint << std::endl;

                Json::Value wpValue = wpEntry["value"];
                
                // Check that 'edges' exists and is an array
                if (!wpValue.isMember("edges") || !wpValue["edges"].isArray())
                {
                    std::cerr << "No valid edges found" << std::endl;
                    continue;
                }
                
                Json::Value edges = wpValue["edges"];

                // Get eta and pt bin edges
                Json::Value etaEdges = edges[0u]; // First array in edges for eta
                Json::Value ptEdges = edges[1];  // Second array in edges for pt
                
                //std::cout << "Processing binning edges" << std::endl;

                // Check if the content exists
                if (!wpValue.isMember("content") || !wpValue["content"].isArray())
                {
                    std::cerr << "No valid content found" << std::endl;
                    continue;
                }

                Json::Value binningContent = wpValue["content"];

                // Loop through the bins and process content
                size_t index = 0;
                for (size_t etaBin = 0; etaBin < etaEdges.size() - 1; etaBin++) // Loop over eta bins
                {
                    for (size_t ptBin = 0; ptBin < ptEdges.size() - 1; ptBin++) // Loop over pt bins
                    {
                        double etaMin = etaEdges[etaBin].asDouble();
                        double etaMax = etaEdges[etaBin + 1].asDouble();
                        double ptMin = ptEdges[ptBin].asDouble();
                        double ptMax = ptEdges[ptBin + 1].asDouble();

                        if (index < binningContent.size()) 
                        {
                            // Get the scale factor for the eta-pt bin
                            double scaleFactor = binningContent[index].asDouble();
                            //std::cout << "Processing eta bin [" << etaMin << ", " << etaMax << "] and pt bin [" << ptMin << ", " << ptMax << "] with scale factor: " << scaleFactor << std::endl;

                            // Construct a key (optional) and add scale factor
                            //std::string key = year + "_" + valType + "_" + workingPoint + "_eta[" + std::to_string(etaMin) + "," + std::to_string(etaMax) + "]_pt[" + std::to_string(ptMin) + "," + std::to_string(ptMax) + "]";
                            //std::cout << "Processing key: " << key << std::endl;
                            switch (systematicType)
                            {
                                case SystematicType::Nominal:
                                    addScaleFactor(etaMin, ptMax, ScaleFactorSet(scaleFactor, 0.0, 0.0));
                                    std::cout << "Adding nominal scale factor for eta bin [" << etaMin << ", " << etaMax << "] and pt bin [" << ptMin << ", " << ptMax << "] with scale factor: " << scaleFactor << std::endl;
                                    break;
                                case SystematicType::Up:
                                    getScaleFactorSet(etaMin, ptMax).systUp = scaleFactor;
                                    std::cout << "Adding up systematic scale factor for eta bin [" << etaMin << ", " << etaMax << "] and pt bin [" << ptMin << ", " << ptMax << "] with scale factor: " << scaleFactor << std::endl;
                                    std::cout << "Readback value of systUp: " << getScaleFactorSet(etaMin, ptMax).systUp << std::endl;
                                    break;
                                case SystematicType::Down:
                                    getScaleFactorSet(etaMin, ptMax).systDown = scaleFactor;
                                    std::cout << "Adding down systematic scale factor for eta bin [" << etaMin << ", " << etaMax << "] and pt bin [" << ptMin << ", " << ptMax << "] with scale factor: " << scaleFactor << std::endl;
                                    break;
                            }
                            //addScaleFactor(etaMin, ptMax, ScaleFactorSet(scaleFactor, 0.0, 0.0));
                            index++; // Move to the next bin content
                        }
                    }
                }
            }        
        }
    }
}

