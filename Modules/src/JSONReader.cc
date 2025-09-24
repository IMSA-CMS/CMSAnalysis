#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include "CMSAnalysis/Modules/interface/JSONReader.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "EventFilter/Utilities/interface/json.h"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

std::map<std::string, ScaleFactor::ScaleFactorSet> JSONReader::readData()
{
    return loadScaleFactorsFromFile(filename);
}


// Function to preprocess the JSON content
std::string JSONReader::preprocessJSON(const std::string &filename) 
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

// ScaleFactor::ScaleFactorSet& JSONReader::getScaleFactorSet(double eta, double pt)
// {
//     // Find the appropriate eta bin
//     auto etaIt = scaleFactors.lower_bound(eta);
//     if (etaIt == scaleFactors.end()) 
//     {
//         throw std::runtime_error("No scale factor found for eta: " + std::to_string(eta));
//     }

//     // Find the appropriate pt bin within the selected eta bin
//     auto ptIt = etaIt->second.lower_bound(pt);
//     if (ptIt == etaIt->second.end()) 
//     {
//         throw std::runtime_error("No scale factor found for pt: " + std::to_string(pt));
//     }

//     return scaleFactors[etaIt->first][ptIt->first];
// }

std::map<std::string, ScaleFactor::ScaleFactorSet> JSONReader::loadScaleFactorsFromFile(std::string filename) 
{
// Preprocess the JSON content
    std::string jsonContent = preprocessJSON(filename);
    if (jsonContent.empty()) {
        return {};
    }

    // Parse the preprocessed JSON content
    std::istringstream jsonStream(jsonContent);
    Json::Value output;
    Json::Reader reader;
    if (!reader.parse(jsonStream, output)) {
        std::cerr << "Failed to parse JSON from file: " << filename << std::endl;
        std::cerr << "Error: " << reader.getFormatedErrorMessages() << std::endl;
        return {};
    }
    //printScaleFactors();
    return loadScaleFactors(output);
    
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
// void JSONReader::printScaleFactors() const
// {
//     //print the electron scale factors
//     std::cout << "Electron Scale Factors:" << std::endl;
//     for (const auto& etaPair : scaleFactors) 
//     {
//         double eta = etaPair.first;
//         for (const auto& ptPair : etaPair.second) 
//         {
//             double pt = ptPair.first;
//             ScaleFactor::ScaleFactorSet scaleFactor = ptPair.second;
//             std::cout << "eta: " << eta << ", pt: " << pt 
//                       << " -> scaleFactor: " << scaleFactor.nominal 
//                       << ", systUp: " << scaleFactor.systUp 
//                       << ", systDown: " << scaleFactor.systDown << std::endl;
//         }
//     }
// }



