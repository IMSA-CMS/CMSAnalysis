#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <string>
#include <vector>
#include <unordered_map>
#include "json/json.h"

JSONScaleFactor::JSONScaleFactor(std::string filename) 
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return;
    }

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(file, root)) {
        std::cerr << "Failed to parse JSON file." << std::endl;
        return;
    }

    const Json::Value& corrections = root["corrections"];
    for (const auto& correction : corrections) {
        const auto& data = correction["data"];
        const auto& absetaEdges = data["edges"];
        const auto& absetaContent = data["content"];

        for (Json::ArrayIndex i = 0; i < absetaEdges.size() - 1; ++i) {
            double etaMin = absetaEdges[i].asDouble();
            double etaMax = absetaEdges[i + 1].asDouble();
            const auto& ptBinning = absetaContent[i];

            const auto& ptEdges = ptBinning["edges"];
            const auto& ptContent = ptBinning["content"];

            for (Json::ArrayIndex j = 0; j < ptEdges.size() - 1; ++j) {
                double ptMin = ptEdges[j].asDouble();
                double ptMax = ptEdges[j + 1].asDouble();
                const auto& scaleFactors = ptContent[j]["content"];

                for (const auto& scaleFactor : scaleFactors) {
                    if (scaleFactor["key"].asString() == "nominal") {
                        double nominal = scaleFactor["value"].asDouble();
                        this->scaleFactors[etaMin][ptMin] = nominal;
                    }
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
        
        for (const auto& etaPair : scaleFactors) {
            if (eta >= etaPair.first && eta < etaPair.second) {
                for (const auto& ptPair : etaPair.second) {
                    if (pt >= ptPair.first && pt < ptPair.second) {
                        eventWeight *= ptPair.second;
                        break;
                    }
                }
                break;
            }
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