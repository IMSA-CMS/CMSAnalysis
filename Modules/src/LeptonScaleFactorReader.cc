#include "CMSAnalysis/Modules/interface/LeptonScaleFactorReader.hh"


	LeptonScaleFactorReader::LeptonScaleFactorReader(std::shared_ptr<ScaleFactorReader> electronReader, std::shared_ptr<ScaleFactorReader> muonReader)
		: electronSF(electronReader), muonSF(muonReader) 
		{

		}

	std::map<std::string, ScaleFactor::ScaleFactorSet> LeptonScaleFactorReader::readData()
	{
		std::map<std::string, ScaleFactor::ScaleFactorSet> data;
		auto electronData = electronSF->readData();
		auto muonData = muonSF->readData();

		// Combine electron and muon data
		for (const auto& entry : electronData) {
			data["electron_" + entry.first] = entry.second;
		}
		for (const auto& entry : muonData) {
			data["muon_" + entry.first] = entry.second;
		}

		return data;
	}

	std::string LeptonScaleFactorReader::getKey(const EventInput* input) const
	{
		// Implement your logic to generate a key based on the input
		return "";
	}

	std::string LeptonScaleFactorReader::getKey(Lepton lepton) const
	{
		// Implement your logic to generate a key based on the lepton
		return "";
	}