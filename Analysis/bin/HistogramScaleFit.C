#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TSystem.h"
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include "RooDataHist.h"
#include "RooArgList.h"
#include "RooRealVar.h"

void fitHistogram(TH1* scaleHist, TH1* fitHist, double xMin, double xMax, double maxScalar)
{
    if (scaleHist->GetNbinsX() != fitHist->GetNbinsX())
    {
        std::cout << "Your histograms don't have the same number of bins!" << std::endl;
    }

    RooRealVar xAxis("xAxis", "xAxis", xMin, xMax);
    RooRealVar scalar("scalar", "scalar", 1, 0, maxScalar);

    //RooRealVar constant("constant", "constant", 1, 1, 1);

    RooPlot* frame = xAxis.frame();

    RooDataHist* scaleData = new RooDataHist("scaleData", "scaleData", xAxis, scaleHist);
    RooDataHist fitData("fitData", "fitData", xAxis, fitHist);

    RooHistPdf curve("curve", "curve", xAxis, xAxis, *scaleData, 0);
    RooExtendPdf scaledCurve("scaledCurve", "scaledCurve", curve, scalar);
    //RooExtendPdf scaledCurve("scaledCurve", "scaledCurve", curve, constant);

    fitData.plotOn(frame);
    scaledCurve.fitTo(fitData);
    scaledCurve.plotOn(frame);
    curve.plotOn(frame);
    frame->Draw();
}

void HistogramScaleFit()
{
    // const std::string filePath = "/uscms/home/cobriend/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/";

    // Simulated data (histogram to scale) comes first, real data (histogram to fit to) comes second
    std::vector<std::string> files = {"noCutSignal.root", "noCutSignal.root"};
    std::vector<std::string> hists = {"1st Highest Lepton Jet Lepton Jet Mass Zoom", "1st Highest Lepton Jet Lepton Jet Mass Zoom"};

    // Change variables here too
    std::vector<HistVariable> scaleVar = {HistVariable::InvariantMass(hists.at(0))};
    std::vector<HistVariable> fitVar = {HistVariable::InvariantMass(hists.at(1))};

    RootFileInput scaleFile(files.at(0), scaleVar);
    RootFileInput fitFile(files.at(1), fitVar);

    TH1* scaleHist = scaleFile.getHistFromName(hists.at(0));
    TH1* fitHist = fitFile.getHistFromName(hists.at(1));

    fitHistogram(scaleHist, fitHist, 0, 10, 100000);
}