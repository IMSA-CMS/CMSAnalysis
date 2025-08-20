#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"

LeptonScaleFactor::LeptonScaleFactor(std::shared_ptr<LeptonScaleFactorReader> reader)
	: ScaleFactor("LeptonScaleFactor", reader)
{
}

	double LeptonScaleFactor::getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const
	{
		// Implement your logic to retrieve the scale factor using the reader
		auto leptons = input->getLeptons();
		double scaleFactor = 1.0;
		for (const auto& lepton : leptons) {
			auto leptonKey = getKey(lepton);
			auto entry = scaleFactors.find(leptonKey);
			if (entry != scaleFactors.end()) {
				scaleFactor *= entry->second.getSystematic(type);
			} else {
				throw std::runtime_error("Scale factor for lepton not found: " + leptonKey);
			}
		}
	}

