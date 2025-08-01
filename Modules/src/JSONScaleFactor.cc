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

JSONScaleFactor::ScaleFactorSet& JSONScaleFactor::getScaleFactorSet(double eta, double pt)
{
    // Find the appropriate eta bin
    auto etaIt = scaleFactors.lower_bound(eta);
    if (etaIt == scaleFactors.end()) 
    {
        throw std::runtime_error("No scale factor found for eta: " + std::to_string(eta));
    }

    // Find the appropriate pt bin within the selected eta bin
    auto ptIt = etaIt->second.lower_bound(pt);
    if (ptIt == etaIt->second.end()) 
    {
        throw std::runtime_error("No scale factor found for pt: " + std::to_string(pt));
    }

    return scaleFactors[etaIt->first][ptIt->first];
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
    jsoncollector::Json::Value output;
    jsoncollector::Json::Reader reader;
    if (!reader.parse(jsonStream, output)) {
        std::cerr << "Failed to parse JSON from file: " << filename << std::endl;
        std::cerr << "Error: " << reader.getFormatedErrorMessages() << std::endl;
        return;
    }

    loadScaleFactors(output);
    printScaleFactors();
}

double JSONScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const 
{
    ScaleFactorSet eventWeight (1.0, 1.0, 1.0);
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
                    if (pt >= ptPair.first) 
                    {
                        
                            eventWeight *= ptPair.second;
                    }
                        found = true;
                        break;
                    
                }
            }
            if (found) break;
        }

        
    }
    
    //std::cout << "eventWeight: " << eventWeight << std::endl;
    if (type == SystematicType::Nominal) {
        return eventWeight.nominal;
    } else if (type == SystematicType::Up) {
        //std::cout << "eventWeight Up: " << eventWeight.systUp << std::endl;
        return eventWeight.systUp;
    } else if (type == SystematicType::Down) {
        //std::cout << "eventWeight Down: " << eventWeight.systDown << std::endl;
        return eventWeight.systDown;
    } else {
        throw std::runtime_error("Invalid systematic type");
    }
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
    //print the electron scale factors
    std::cout << "Electron Scale Factors:" << std::endl;
    for (const auto& etaPair : scaleFactors) 
    {
        double eta = etaPair.first;
        for (const auto& ptPair : etaPair.second) 
        {
            double pt = ptPair.first;
            ScaleFactorSet scaleFactor = ptPair.second;
            std::cout << "eta: " << eta << ", pt: " << pt 
                      << " -> scaleFactor: " << scaleFactor.nominal 
                      << ", systUp: " << scaleFactor.systUp 
                      << ", systDown: " << scaleFactor.systDown << std::endl;
        }
    }
}

void JSONScaleFactor::addScaleFactor(double eta, double pt, ScaleFactorSet scaleFactor) 
{
    scaleFactors[eta][pt] = scaleFactor;
}




