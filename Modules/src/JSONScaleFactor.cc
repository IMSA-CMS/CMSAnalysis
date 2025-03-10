#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "EventFilter/Utilities/interface/json.h"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

// Function to preprocess the JSON content
std::string preprocessJSON(const std::string &filename) 
{
    std::ifstream file(Utility::getFullPath(filename));
    if (!file.is_open()) 
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    // Replace Infinity and NaN with specific values
    jsonContent = std::regex_replace(jsonContent, std::regex("Infinity"), "1e30"); // or "null"
    jsonContent = std::regex_replace(jsonContent, std::regex("NaN"), "null");

    return jsonContent;
}

JSONScaleFactor::JSONScaleFactor(std::string filename, SystematicType systematicType) : systematicType(systematicType) 
{

}

void JSONScaleFactor::loadScaleFactorsFromFile(std::string filename) 
{
// Preprocess the JSON content
    std::string jsonContent = preprocessJSON(filename);
    if (jsonContent.empty()) {
        return;
    }

    // Parse the preprocessed JSON content
    std::istringstream jsonStream(jsonContent);
    Json::Value output;
    Json::Reader reader;
    if (!reader.parse(jsonStream, output)) {
        std::cerr << "Failed to parse JSON from file: " << filename << std::endl;
        std::cerr << "Error: " << reader.getFormatedErrorMessages() << std::endl;
        return;
    }

    loadScaleFactors(output);
    printScaleFactors();
}

double JSONScaleFactor::getScaleFactor(const EventInput* input) const 
{
    double eventWeight = 1.0;
    for (auto lepton : getParticles(input)) 
    {
        double pt = lepton.getPt();
        double eta = lepton.getEta();
        
        //std::cout << "pt: " << pt << ", eta: " << eta << std::endl;

        bool found = false;
        for (const auto& etaPair : scaleFactors) {
            if (eta >= etaPair.first) {
                //std::cout << "eta: " << eta << std::endl;
                for (const auto& ptPair : etaPair.second) {
                    if (pt >= ptPair.first) {
                        //std::cout << "pt: " << pt << std::endl;
                        eventWeight *= ptPair.second;
                        found = true;
                        break;
                    }
                }
            }
            if (found) break;
        }

        if (!found) {
            eventWeight *= 1.0;
        }
    }
    
    //std::cout << "eventWeight: " << eventWeight << std::endl;

    return eventWeight;
}

// void JSONScaleFactor::printScaleFactors() const 
// {
//     for (const auto& etaPair : scaleFactors) {
//         double eta = etaPair.first;
//         for (const auto& ptPair : etaPair.second) {
//             double pt = ptPair.first;
//             double scaleFactor = ptPair.second;
//             std::cout << "eta: " << eta << ", pt: " << pt 
//                       << " -> scaleFactor: " << scaleFactor << std::endl;
//         }
//     }
// }
void JSONScaleFactor::printScaleFactors() const
{
    
}

void JSONScaleFactor::addScaleFactor(double eta, double pt, double scaleFactor)
{
    scaleFactors[eta][pt] = scaleFactor;
}



