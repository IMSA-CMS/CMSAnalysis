#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <map>
#include <float.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"

#define _USE_MATH_DEFINES

void higgsCompleteAnalysisTest()
{
	auto analysis = std::make_shared<HiggsCompleteAnalysis>();
	std::vector<int> masses = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400};
	TCanvas* canvas = new TCanvas("c1");

	for (int mass : masses) 
	{
		auto channelTest = analysis->getChannel("eeuu");
		auto higgsProcess = channelTest->findProcess("Higgs Signal " + std::to_string(mass));
		TH1* signalHist = higgsProcess->getHist("Same Sign Inv Mass");
		signalHist->DrawCopy();
	}

	auto backgroundChannel = analysis->getChannel("eeuu");
	auto zz_background = backgroundChannel->findProcess("ZZ Background");
	// zzHist->DrawCopy();
}
