#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"

#include <iostream>
#include <memory>	
#include "TCanvas.h"

void SuperPlot()
{
	std::string plotName = "Invariant Mass";
	std::string outFile = "SuperPlot.png";

	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Private Work (CMS Simulation/Data)");
	plotFormatter->setUpperMasslimit(.5);
	plotFormatter->setRebinFactor(5);
	plotFormatter->setFirstBin(-1);

	// HistVariable nameVar(ParticleType::leptonJet(), 1, HistVariable::VariableType::LeptonJetDeltaR, "Lepton Jet #DeltaR");
	// auto ana = std::make_shared<HiggsCompleteAnalysis>();
	// TCanvas *canvas = plotFormatter->completePlot(ana, nameVar, xAxisTitle, yAxisTitle, false, true, false, "eeee", "Higgs Group 1000");
	HistVariable nameVar(HistVariable::VariableType::DarkPhotonBDTOutput);

	//auto ana = std::make_shared<HiggsCompleteAnalysis>();
	plotFormatter->setRebinFactor(5);
	auto ana = std::make_shared<DarkPhotonCompleteAnalysis>();
	TCanvas *canvas = plotFormatter->completePlot(ana, nameVar, "BDT Output", "Events", false, false, true, "Low Mass and Same Sign/1Jet");

	// auto ana = std::make_shared<DarkPhotonCompleteAnalysis>();
	// TCanvas *canvas = plotFormatter->completePlot(ana, nameVar, xAxisTitle, yAxisTitle, false, false, true, "High Mass and Different Sign/1Jet");

	//Uncomment to save a png picture in your bin folder
	//canvas->SaveAs(outFile.c_str());
}