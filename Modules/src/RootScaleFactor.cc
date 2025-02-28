#include "CMSAnalysis/Modules/interface/RootScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

#include "TH1.h"
#include "TFile.h"

RootScaleFactor::RootScaleFactor (std::string filename, std::string histname) 
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

double RootScaleFactor::getScaleFactor(const EventInput* input) const
{
	auto pileUp = input->getNumPileUpInteractions();
	
	int bin = std::max(1, std::min(histogram->GetNbinsX(), histogram->GetXaxis()->FindBin(pileUp)));
    return histogram->GetBinContent(bin);
}