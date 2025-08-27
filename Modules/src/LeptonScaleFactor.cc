#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"

LeptonScaleFactor::LeptonScaleFactor(std::shared_ptr<LeptonScaleFactorReader> reader)
	: ScaleFactor("LeptonScaleFactor", reader)
{
}

std::vector<std::string> LeptonScaleFactor::getKey(const EventInput* input) const
{
	// Implement your logic to retrieve the scale factor using the reader
	auto particles = input->getParticles(EventInput::RecoLevel::Reco, particleType);

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
