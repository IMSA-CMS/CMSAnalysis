#include "CMSAnalysis/Modules/interface/PileUpRootReader.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "TH1.h"
#include "TFile.h"

    PileUpRootReader::PileUpRootReader(std::string datafilename, std::string mcfilename, std::string datahistname,
         std::string mchistname, std::string datahistname_up, std::string datahistname_down)
{
	TH1::AddDirectory(kFALSE);

	auto datafile = TFile::Open(Utility::getFullPath(datafilename).c_str()); 
	if (!datafile)
	{
		throw std::runtime_error("RootScaleFile " + datafilename + " does not exist.");
	}

	auto datahistogram = datafile->Get<TH1>(datahistname.c_str());
	if (!datahistogram)
	{
		throw std::runtime_error("Histogram " + datahistname + " does not exist.");
	}
	auto datahistogramUp = datafile->Get<TH1>(datahistname_up.c_str());
	if (!datahistogramUp)
	{
		throw std::runtime_error("Histogram " + datahistname_up + " does not exist.");
	}

	auto datahistogramDown = datafile->Get<TH1>(datahistname_down.c_str());
	if (!datahistogramDown)
	{
		throw std::runtime_error("Histogram " + datahistname_down + " does not exist.");
	}
	auto mcfile = TFile::Open(Utility::getFullPath(mcfilename).c_str()); 
	if (!mcfile)
	{
		throw std::runtime_error("RootScaleFile " + mcfilename + " does not exist.");
	}
	auto mchistogram = mcfile->Get<TH1>(mchistname.c_str());
	if (!mchistogram)
	{
		throw std::runtime_error("Histogram " + mchistname + " does not exist.");
	}

	if (mchistogram->GetNbinsX() != datahistogram->GetNbinsX())
	{
		throw std::runtime_error("Data and MC histograms have different number of bins.");
	}

	histogram = dynamic_cast<TH1*>(datahistogram->Clone("Pile up ratio histogram"));
	double scalefactor = 1/histogram->Integral();
	histogram->Scale(scalefactor);
	mchistogram->Scale(1/mchistogram->Integral());
	histogram->Divide(mchistogram);
	up = dynamic_cast<TH1*>(datahistogramUp->Clone("Pile up ratio histogram up"));
	up->Scale(scalefactor);
	up->Divide(mchistogram);
	down = dynamic_cast<TH1*>(datahistogramDown->Clone("Pile up ratio histogram down"));
	down->Scale(scalefactor);
	down->Divide(mchistogram);

	delete datahistogram;
	delete datahistogramUp;
	delete datahistogramDown;
	delete mchistogram;
	delete datafile;
	delete mcfile;
}

std::map<std::string, ScaleFactor::ScaleFactorSet> PileUpRootReader::readData()
{
	std::map<std::string, ScaleFactor::ScaleFactorSet> scaleFactors;
	// Read the histogram and fill the scaleFactors map
	for (int i = 1; i <= histogram->GetNbinsX(); ++i)
	{
		std::string key = std::to_string(histogram->GetXaxis()->GetBinLowEdge(i));
		double value = histogram->GetBinContent(i);
		double valueUp = up->GetBinContent(i);
		double valueDown = down->GetBinContent(i);
		scaleFactors[key] = ScaleFactor::ScaleFactorSet(value, valueUp, valueDown);
	}
	return scaleFactors;
}