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
	return par[0] * std::pow(std::exp(1), -(x[0] + par[2]) * par[1]);
}

void dileptonCurveFitting()
{
	std::vector<char const*> fileNames{"egdata.root"};

	int counter = 0;

	for (auto filename : fileNames)
	{
		std::string rootName(filename);

		// figuring out parameters
		std::cout << "fitting to " << filename << "\n";
		TFile* file = TFile::Open(filename);
		TH1* histss = file->Get<TH1>("same_Sign_Invariant_Mass");
		TH1* histos = file->Get<TH1>("invariant_Mass");

		int nBins = histss->GetNbinsX();

		// std::cout << nBins << "\n";

		std::vector<double> x, y, mx, my;
		for (int i = 1; i <= nBins; i++)
		{
			if (histss->GetBinCenter(i) > 80 && histss->GetBinCenter(i) < 100)
			{
				mx.push_back(histss->GetBinCenter(i));
				my.push_back(histss->GetBinContent(i));
				continue;
			}

			x.push_back(histss->GetBinCenter(i));
			y.push_back(histss->GetBinContent(i));
		}



		auto graph = new TGraph(x.size(), x.data(), y.data());
		graph->SetTitle("same same invariant mass; m; events");
		graph->Draw("AP*");

		TF1 *g0 = new TF1("g0", exponential, 32, 400, 3);
		g0->SetParameters(1, 1, -20);
		TFitResultPtr fitss = graph->Fit(g0, "S", "", 32, 400);
		// get the tf1 object from pointer
		TF1 *fitted = graph->GetFunction("g0");

		auto area1 = histss->Integral(histss->FindFixBin(80), histss->FindFixBin(100), "width"); // 148550, 
		auto area2 = fitted->Integral(80, 100); // 60670.4, 
		
		// diff = 87879.6
		std::cout << area1 << "\t" << area2 << "\t" << area1 - area2 << "\n";

		counter++;
	}
}