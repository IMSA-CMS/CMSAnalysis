#include "CMSAnalysis/Modules/interface/MuonJSONReader.hh"

MuonJSONReader::MuonJSONReader(std::string filename) : JSONReader(filename)
{
	loadScaleFactorsFromFile(filename);
}
 std::map<std::string, ScaleFactor::ScaleFactorSet> MuonJSONReader::loadScaleFactors(Json::Value output)
{
    std::map<std::string, ScaleFactor::ScaleFactorSet> scaleFactorMap;

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
            double nominal, systUp, systDown;
            for (size_t j = 0; j < allsfContent.size(); j++) {
                Json::Value key = allsfContent[j]["key"];
                
                if (key.asString() == "nominal") {
                    nominal = allsfContent[j]["value"].asDouble();
                
                }
                else if (key.asString() == "systup") {
                    systUp = allsfContent[j]["value"].asDouble();
                }
                else if (key.asString() == "systdown") {
                    systDown = allsfContent[j]["value"].asDouble();
                }
            }
            std::string etaMinString = std::to_string(eta_min);
            std::string ptMaxString = std::to_string(pt_max);
            scaleFactorMap[etaMinString + "_" + ptMaxString] = (scaleFactor, systUp, systDown);
            //std::cout << "Adding nominal scale factor for eta bin [" << eta_min << ", " << etaMax << "] and pt bin [" << ptMin << ", " << ptMax << "] with scale factor: " << scaleFactor << std::endl;

        }
    }

    return scaleFactorMap;
}

std::string getKey(Lepton lepton)
{
    // Go through scaleFactorMap
    // Find highest eta key less than your eta and lowest pT key more than your pT in the keys.

    // review this code
    std::string bestKey = "";
    double bestEta = -1;
    double bestPt = std::numeric_limits<double>::max();

    for (const auto& entry : scaleFactorMap) {
        std::string key = entry.first;
        size_t underscorePos = key.find("_");
        double eta = std::stod(key.substr(0, underscorePos));
        double pt = std::stod(key.substr(underscorePos + 1));

        if (eta < lepton.eta && pt > lepton.pt) {
            if (eta > bestEta || (eta == bestEta && pt < bestPt)) {
                bestKey = key;
                bestEta = eta;
                bestPt = pt;
            }
        }
    }

    return bestKey;
}