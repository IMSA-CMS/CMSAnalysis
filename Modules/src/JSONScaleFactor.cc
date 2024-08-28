#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "EventFilter/Utilities/interface/json.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

// Function to preprocess the JSON content
std::string preprocessJSON(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
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

JSONScaleFactor::JSONScaleFactor(std::string filename) 
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
    for (auto memberName : output.getMemberNames()) {
        std::cout << "memberName: " << memberName << std::endl;
    }
    Json::Value allCorrections = output["corrections"];
    Json::Value allData = allCorrections[0u]["data"];
    Json::Value allEdges = allData["edges"];
    Json::Value allContent = allData["content"];

    for (size_t i = 0; i < allEdges.size(); i++) {
        double eta_min = allEdges[i].asDouble();

        Json::Value allptEdges = allContent[i]["edges"];
        Json::Value allptContent = allContent[i]["content"];

        for (size_t k = 0; k < allptEdges.size(); k++) {
            double pt_max = allptEdges[k].asDouble();
            Json::Value allsfContent = allptContent[k]["content"];

            for (size_t j = 0; j < allsfContent.size(); j++) {
                Json::Value key = allsfContent[j]["key"];
                if (key.asString() == "nominal") {
                    double scaleFactor = allsfContent[j]["value"].asDouble();
                    scaleFactors[eta_min][pt_max] = scaleFactor;
                }
            }
        }
    }
    printScaleFactors();
}

double JSONScaleFactor::getScaleFactor(const EventInput* input) const 
{
    double eventWeight = 1.0;
    for (auto lepton : input->getLeptons(EventInput::RecoLevel::Reco)) 
    {
        double pt = lepton.getPt();
        double eta = lepton.getEta();
        
        bool found = false;
        for (const auto& etaPair : scaleFactors) {
            if (eta >= etaPair.first) {
                for (const auto& ptPair : etaPair.second) {
                    if (pt >= ptPair.first) {
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
    return eventWeight;
}

void JSONScaleFactor::printScaleFactors() const 
{
    for (const auto& etaPair : scaleFactors) {
        double eta = etaPair.first;
        for (const auto& ptPair : etaPair.second) {
            double pt = ptPair.first;
            double scaleFactor = ptPair.second;
            std::cout << "eta: " << eta << ", pt: " << pt 
                      << " -> scaleFactor: " << scaleFactor << std::endl;
        }
    }
}


