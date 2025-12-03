#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TCanvas.h"
#include "TH1.h"
#include "THStack.h"
#include "TLegend.h"
#include <TF1.h>
#include <TFile.h>
#include <TH2.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

// Global variables to be used in the chi-squared function
// (dyn bg, fixed bg, observed)
std::vector<std::tuple<TH1 *, TH1 *, TH1 *>> globalData;

double error(const double *const x, const double *const par)
{
    const double dyn = par[0];
    const double fix = par[1];

    double sum = 0;
    for (const auto channel : globalData)
    {
        auto *const dynBg = std::get<0>(channel);
        auto *const fixedBg = std::get<1>(channel);
        auto *const data = std::get<2>(channel);
        const auto pred =
            dyn * dynBg->GetBinContent(dynBg->FindBin(x[0])) + fix * fixedBg->GetBinContent(fixedBg->FindBin(x[0]));
        const auto obsv = data->GetBinContent(data->FindBin(x[0]));
        const auto err = data->GetBinError(data->FindBin(x[0]));
        if (err != 0)
        {
            sum += (pred - obsv) * (pred - obsv) / (err * err);
        }
    }

    // std::cout << dyn << "," << fix << "," << x[0] << "->" << sum << "\n";

    return sqrt(sum);
}

void BackgroundScaleSimultaneous(const char *const outputName)
{
    const std::string dynamicBgName = "Drell-Yan Background";
    const std::vector<std::string> channelNames{"e_e__ZPeak", "u_u__ZPeak"};

    auto outFile = TFile(outputName, "RECREATE");

    auto analysis = HiggsCompleteAnalysis();

    const HistVariable histVar(HistVariable::VariableType::RecoOppositeSignInvariantMass);

    for (const auto &channelName : channelNames)
    {
        const auto channel = analysis.getChannel(channelName);

        // Retrieve histograms
        auto *data = channel->getHists(histVar, Channel::Label::Data).at(0);
        auto *dynBg = analysis.getHist(histVar, dynamicBgName, false, channelName);
        auto *fixedBg = dynamic_cast<TH1 *>(dynBg->Clone());
        fixedBg->Reset();

        for (const std::string &name : channel->getNamesWithLabel(Channel::Label::Background))
        {
            if (name == dynamicBgName)
            {
                continue;
            }
            std::cout << "Fixed bg name: " << name << "\n";
            TH1 *fixedHist = analysis.getHist(histVar, name, true, channelName);
            fixedBg->Add(fixedHist);
        }

        auto *canvas = new TCanvas((channelName + " Initial Stacked Histogram").c_str(),
                                   (channelName + " Initial Stacked Histogram").c_str(), 800, 600);
        auto stack = THStack("stack", (channelName + " Initial Background Components;Mass;Events").c_str());

        dynBg->SetFillColor(kRed);
        fixedBg->SetFillColor(kBlue);
        data->SetMarkerStyle(20);

        stack.Add(fixedBg);
        stack.Add(dynBg);

        stack.Draw("HIST");
        data->Draw("E SAME");

        auto legend = TLegend(0.7, 0.7, 0.9, 0.9);
        legend.AddEntry(data, "Data", "lep");
        legend.AddEntry(fixedBg, "Fixed Background", "f");
        legend.AddEntry(dynBg, ("Dynamic Background (" + dynamicBgName + ")").c_str(), "f");
        legend.Draw();

        outFile.WriteObject(canvas, canvas->GetName());
        globalData.push_back(std::tuple(dynBg, fixedBg, data));
    }

    // Draw initial stacked histogram

    auto *const func = new TF1("f1", error, 0, 1, 2, 1, TF1::EAddToList::kNo);
    func->SetParameters(1, 1);
    func->SetParLimits(0, 0, 100);
    func->SetParLimits(1, 0, 100);
    func->FixParameter(1, 1);
    auto *const hist = dynamic_cast<TH1 *>(std::get<2>(globalData.at(0))->Clone());
    hist->Reset();

    hist->Fit(func, "WW");

    const auto dynScale = func->GetParameter(0);
    const auto fixScale = func->GetParameter(1);

    std::cout << "-------------------------\n";
    std::cout << "Best fit scale factors:\n";
    std::cout << "Dynamic Background Scale: " << dynScale << "\n";
    std::cout << "Fixed Background Scale: " << fixScale << "\n";
    std::cout << "Chi-sqared: " << func->GetChisquare() << '\n';
    std::cout << "-------------------------\n";

    for (size_t i = 0; i < channelNames.size(); i++)
    {
        const auto &name = channelNames.at(i);
        const auto *canvas =
            new TCanvas((name + " BestFitStack").c_str(), (name + " Best Fit Stacked Histogram").c_str(), 800, 600);
        auto bestFitStack = THStack("bestFitStack", (name + " Best Fit Background Components;Mass;Events").c_str());

        const auto hists = globalData.at(i);

        TH1 *const bestFitDynamicBG = dynamic_cast<TH1 *>(std::get<0>(hists)->Clone());
        bestFitDynamicBG->Scale(dynScale);
        bestFitDynamicBG->SetFillColor(kRed);

        TH1 *const bestFitFixedBG = dynamic_cast<TH1 *>(std::get<1>(hists)->Clone());
        bestFitFixedBG->Scale(fixScale);
        bestFitFixedBG->SetFillColor(kBlue);

        bestFitStack.Add(bestFitFixedBG);
        bestFitStack.Add(bestFitDynamicBG);

        bestFitStack.Draw("HIST");
        std::get<2>(hists)->Draw("E SAME");

        auto bestFitLegend = TLegend(0.7, 0.7, 0.9, 0.9);
        bestFitLegend.AddEntry(std::get<2>(hists), "Data", "lep");
        bestFitLegend.AddEntry(bestFitFixedBG, "Fixed Background", "f");
        bestFitLegend.AddEntry(bestFitDynamicBG, ("Dynamic Background (Best fit, " + dynamicBgName + ")").c_str(), "f");
        bestFitLegend.Draw();

        outFile.WriteObject(canvas, canvas->GetName());
    }
}
