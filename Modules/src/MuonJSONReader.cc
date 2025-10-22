#include "CMSAnalysis/Modules/interface/MuonJSONReader.hh"

using jsoncollector::Json::Value;

MuonJSONReader::MuonJSONReader(std::string filename) : JSONReader(filename)
{

}
 std::map<std::string, ScaleFactor::ScaleFactorSet> MuonJSONReader::loadScaleFactors(Value output)
{
    std::map<std::string, ScaleFactor::ScaleFactorSet> scaleFactorMap;

    Value allCorrections = output["corrections"];
    Value allData = allCorrections[0u]["data"];
    Value allEdges = allData["edges"];
    Value allContent = allData["content"];

    for (size_t i = 0; i < allEdges.size(); i++) {
        double eta_min = allEdges[i].asDouble();

        Value allptEdges = allContent[i]["edges"];
        Value allptContent = allContent[i]["content"];

        for (size_t k = 0; k < allptEdges.size(); k++) {
            double pt_max = allptEdges[k].asDouble();
            Value allsfContent = allptContent[k]["content"];
            double nominal, systUp, systDown;
            for (size_t j = 0; j < allsfContent.size(); j++) {
                Value key = allsfContent[j]["key"];
                
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
            scaleFactorMap[etaMinString + "_" + ptMaxString] = ScaleFactor::ScaleFactorSet(nominal, systUp, systDown);
            //std::cout << "Adding nominal scale factor for eta bin [" << eta_min << "] and pt bin [" << pt_max << "] with scale factor: " << nominal << std::endl;

        }
    }

    return scaleFactorMap;
}

// std::string MuonJSONReader::getKey(Lepton lepton)
// {
//     // Go through scaleFactorMap
//     // Find highest eta key less than your eta and lowest pT key more than your pT in the keys.

//     // review this code
//     std::string bestKey = "";
//     double bestEta = -1;
//     double bestPt = -1;

//     for (const auto& entry : scaleFactorMap) {
//         std::string key = entry.first;
//         size_t underscorePos = key.find("_");
//         double eta = std::stod(key.substr(0, underscorePos));
//         double pt = std::stod(key.substr(underscorePos + 1));

//         if (eta < lepton.getEta() && pt > lepton.getPt()) {
//             if (eta > bestEta || (eta == bestEta && pt < bestPt)) {
//                 bestKey = key;
//                 bestEta = eta;
//                 bestPt = pt;
//             }
//         }
//     }

//     return bestKey;
// }