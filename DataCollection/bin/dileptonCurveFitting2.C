#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TGraphErrors.h"

Double_t exponential(Double_t *x, Double_t *par)
{
	// power law
	return par[0] * std::pow(std::exp(1), -(x[0] + par[2]) * par[1]);
	// expoenential
	// return par[0] * std::exp(-(x[0] + par[2]) * par[1]); 
}

void dileptonCurveFitting2()
{
	std::vector<char const*> fileNames{"electroniv.root"};

	int counter = 0;

	for (auto filename : fileNames)
	{
		std::string rootName(filename);

		// figuring out parameters
		std::cout << "fitting to " << filename << "\n";
		TFile* file = TFile::Open(filename);
		TH1* histos = file->Get<TH1>("invariant_Mass");

		int nBins = histos->GetNbinsX();

		// std::cout << nBins << "\n";

		std::vector<double> x, y, mx, my;
		for (int i = 1; i <= nBins; i++)
		{
			if (histos->GetBinCenter(i) > 30 && histos->GetBinCenter(i) < 150)
			{
				mx.push_back(histos->GetBinCenter(i));
				my.push_back(histos->GetBinContent(i));
				continue;
			}

			x.push_back(histos->GetBinCenter(i));
			y.push_back(histos->GetBinContent(i));
		}


		auto graph = new TGraph(x.size(), x.data(), y.data());
		graph->SetTitle("invariant mass; m; events");
		graph->Draw("AP*");

		TF1 *g0 = new TF1("g0", exponential, 0, 400, 3);
		g0->SetParameters(10, 0.5, -10);
		TFitResultPtr fitos = graph->Fit(g0, "S", "", 0, 400);
		// get the tf1 object from pointer
		TF1 *fitted = graph->GetFunction("g0");

		auto area1 = histos->Integral(histos->FindFixBin(30), histos->FindFixBin(150));   //7.06644e06
		auto area2 = fitted->Integral(30, 150); // 355559
		// diff = 6.71088e06
		std::cout << area1 << "\t" << area2 << "\t" << area1 - area2 << "\n";

		counter++;
	}
}