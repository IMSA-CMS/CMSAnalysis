#include "CMSAnalysis/Modules/interface/MuonScaleFactor.hh"

MuonScaleFactor::MuonScaleFactor(std::string filename) : JSONScaleFactor(filename)
{
	loadScaleFactorsFromFile(filename);
}
ParticleCollection<Particle> MuonScaleFactor::getParticles(const EventInput* input) const
{
    return input->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());
}
void MuonScaleFactor::loadScaleFactors(jsoncollector::Json::Value output)
{
    auto allCorrections = output["corrections"];
    auto allData = allCorrections[0u]["data"];
    auto allEdges = allData["edges"];
    auto allContent = allData["content"];

    for (size_t i = 0; i < allEdges.size(); i++) {
        double eta_min = allEdges[i].asDouble();

        auto allptEdges = allContent[i]["edges"];
        auto allptContent = allContent[i]["content"];

        for (size_t k = 0; k < allptEdges.size(); k++) {
            double pt_max = allptEdges[k].asDouble();
            auto allsfContent = allptContent[k]["content"];
            double nominal, systUp, systDown;
            for (size_t j = 0; j < allsfContent.size(); j++) {
                auto key = allsfContent[j]["key"];
                
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
            addScaleFactor(eta_min, pt_max, ScaleFactorSet(nominal, systUp, systDown));
        }
    }
}