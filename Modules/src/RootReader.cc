#include "CMSAnalysis/Modules/interface/RootReader.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

#include "TH1.h"
#include "TFile.h"

RootReader::RootReader (std::string filename, std::string histname) 
{
	auto file = TFile::Open(filename.c_str()); 
	if (!file)
	{
		throw std::runtime_error("RootScaleFile " + filename + " does not exist.");
	}
	histogram = file->Get<TH1>(histname.c_str());
	if (!histogram)
	{
		throw std::runtime_error("Histogram " + histname + " does not exist.");
	}
}

std::map<std::string, ScaleFactor::ScaleFactorSet> RootReader::readData()
{
	std::map<std::string, ScaleFactor::ScaleFactorSet> scaleFactors;

	// Read the histogram and fill the scaleFactors map
	for (int i = 1; i <= histogram->GetNbinsX(); ++i)
	{
		std::string key = std::to_string(histogram->GetXaxis()->GetBinLowEdge(i));
		double value = histogram->GetBinContent(i);
		scaleFactors[key] = ScaleFactor::ScaleFactorSet{ value };
	}

	return scaleFactors;
}