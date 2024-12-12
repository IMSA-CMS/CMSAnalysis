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
#include <fmt/core.h>

void RunSuperimpose() {
    try {
        std::cout << "Starting RunSuperimpose..." << std::endl;

        auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary Data");
        const std::string filePath = "/afs/cern.ch/user/c/ceddingt/CMSSW_14_0_1/src/CMSAnalysis/Output/";
        std::vector<std::string> files = {"testscalefactors.root", "testnoscalefactors.root"};
        std::vector<std::string> hists = {
            "eeuu__hists/eeuu_Reco Same Sign Invariant Mass",
            "eeuu__hists/eeuu_Reco Same Sign Invariant Mass"
        };
        std::vector<TString> names = {"Scale Factor", "No Scale Factor"};
        std::vector<int> colors = {1, 2, 3};
        TString xTitle = "Invariant Mass (GeV)";
        TString yTitle = "Event Ratio";

        int count = 0;
        TFile* openedFile = nullptr;
        TH1* hist = nullptr;
        std::vector<TH1*> histVector;
        std::string fileName;

        for (const std::string& file : files) {
            fileName = filePath + file;
            std::cout << "Opening file: " << fileName << std::endl;
            openedFile = TFile::Open(fileName.c_str(), "read");
            if (!openedFile) {
                throw std::runtime_error("Cannot open file: " + fileName);
            }

            hist = dynamic_cast<TH1*>(openedFile->Get(hists.at(count).c_str()));
            if (!hist) {
                throw std::runtime_error("Histogram " + hists.at(count) + " not found in file: " + fileName);
            }

            if (dynamic_cast<TH2*>(hist)) {
                std::cout << "Converting 2D histogram to 1D..." << std::endl;
                TH2* hist2D = dynamic_cast<TH2*>(hist);
                TH1* newhist = hist2D->ProjectionX("_px", 0, -1, "E");
                histVector.push_back(newhist);
            } else {
                histVector.push_back(hist);
            }

            std::cout << "Histogram loaded successfully: " << hists.at(count) << std::endl;
            count++;
        }

        for (int i = 0; i < count; i++) {
            std::cout << "Rebinning histogram " << i << " with factor 10..." << std::endl;
            histVector[i]->Rebin(10);
        }

        double average[count] = {0};
        for (int i = 0; i < count; i++) {
            std::cout << "Processing histogram " << i << " for ratio calculation..." << std::endl;
            for (int j = 1; j <= histVector[i]->GetNbinsX(); j++) {
                double histValue = histVector[i]->GetBinContent(j);
                double baseValue = histVector[1]->GetBinContent(j); // Assuming the 2nd file is the base

                if (baseValue == 0) {
                    histVector[i]->SetBinContent(j, 0);
                    average[i] += 0;
                } else {
                    double ratio = (histValue - baseValue) / baseValue;
                    histVector[i]->SetBinContent(j, ratio);
                    average[i] += ratio;
                }

                std::cout << fmt::format("Bin {}: histValue = {:.3f}, baseValue = {:.3f}, ratio = {:.3f}", j, histValue, baseValue, ratio) << std::endl;
            }
            std::cout << "Average ratio for histogram " << i << ": " << average[i] / histVector[i]->GetNbinsX() * 100 << "%" << std::endl;
        }

        TCanvas* canvas = plotFormatter->noScaleSimpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);
        std::cout << "Canvas created successfully." << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}

void SuperimposeRatio() {
    std::cout << "Starting SuperimposeRatio..." << std::endl;
    RunSuperimpose();
    std::cout << "SuperimposeRatio completed." << std::endl;
}
