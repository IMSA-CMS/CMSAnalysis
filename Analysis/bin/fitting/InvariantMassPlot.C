#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "TH1.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLegend.h"
#include "TBox.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TStyle.h"
#include <iostream>
#include <string>
#include <cmath>

// Configuration for the best representative channel
const int TEST_MASS = 500;
const std::string CHANNEL_NAME = "euuu";
const std::string PROJ_SUFFIX = "_y";
const double WINDOW_HALFWIDTH = 40.0;

// Output file names
const std::string OUTPUT_PNG = "InvariantMassPlot.png";
const std::string OUTPUT_PDF = "InvariantMassPlot.pdf";

// Background process names (as seen in HiggsCompleteAnalysis)
const std::vector<std::pair<std::string, std::string>> BACKGROUNDS = {
    {"Drell-Yan Background",            "Drell-Yan"},
    {"QCD Background",                  "QCD"},
    {"ZZ Background",                   "ZZ"},
    {"WJets Background",                "WJets"},
    {"t#bar{t}, Multiboson Background", "ttbar/Multiboson"}
};

void InvariantMassPlot()
{
    std::cout << "Creating invariant mass plot for " << CHANNEL_NAME << PROJ_SUFFIX 
              << " at " << TEST_MASS << " GeV...\n";

    // Load analysis framework
    auto analysis = HiggsCompleteAnalysis();
    
    // Get the channel
    auto channel = analysis.getChannel(CHANNEL_NAME);
    if (!channel)
    {
        std::cerr << "ERROR: Could not find channel " << CHANNEL_NAME << "\n";
        return;
    }

    // Set up HistVariable for Y projection (the _y configuration)
    const HistVariable histVar(HistVariable::VariableType::InvariantMass, "", true, false);
    
    // Get signal process for this channel
    const auto signalProcess = channel->findProcess(
        "Higgs signal " + CHANNEL_NAME + " " + std::to_string(TEST_MASS));
    if (!signalProcess)
    {
        std::cerr << "ERROR: Could not find signal process\n";
        return;
    }

    TH1 *signalHist = signalProcess->getHist(histVar, true);
    if (!signalHist || signalHist->GetEntries() == 0)
    {
        std::cerr << "ERROR: Signal histogram is empty\n";
        return;
    }

    std::cout << "Got signal histogram with " << signalHist->GetEntries() << " entries\n";

    // Sum all background histograms
    TH1 *totalBgHist = nullptr;
    
    for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
    {
        const auto bgProcess = channel->findProcess(BACKGROUNDS[i].first);
        if (!bgProcess)
            continue;

        TH1 *bgHist = bgProcess->getHist(histVar, true);
        if (!bgHist || bgHist->GetEntries() == 0)
            continue;

        std::cout << "Adding " << BACKGROUNDS[i].first << " with " << bgHist->GetEntries() << " entries\n";

        if (!totalBgHist)
        {
            // Clone the first background to use as total
            totalBgHist = (TH1*)bgHist->Clone("totalBackground");
            totalBgHist->SetDirectory(nullptr);
        }
        else
        {
            totalBgHist->Add(bgHist);
        }
    }

    if (!totalBgHist)
    {
        std::cerr << "ERROR: No background histogram found\n";
        return;
    }

    std::cout << "Total background histogram has " << totalBgHist->GetEntries() << " entries\n";

    // Disable statistics box
    gStyle->SetOptStat(0);

    // Create canvas - matching WindowAnalysis dimensions
    TCanvas *canvas = new TCanvas("c1", "Invariant Mass Distribution", 1200, 800);
    canvas->SetLeftMargin(0.12);
    canvas->SetRightMargin(0.05);
    canvas->SetBottomMargin(0.12);
    canvas->SetTopMargin(0.16);  // More space for labels at top
    canvas->SetGridx(true);
    canvas->SetGridy(true);

    // Set up styling for histograms - neon blue and deep yellow
    const int signalColor = TColor::GetColor("#0000FF");  // neon blue (pure blue)
    const int bgColor = TColor::GetColor("#CCAA00");      // deep yellow
    const int boxColor = TColor::GetColor("#ffff00");     // yellow for window box

    signalHist->SetLineColor(signalColor);
    signalHist->SetMarkerColor(signalColor);
    signalHist->SetMarkerStyle(20);
    signalHist->SetMarkerSize(1.2);
    signalHist->SetLineWidth(3);
    signalHist->SetFillColor(0);

    totalBgHist->SetLineColor(bgColor);
    totalBgHist->SetMarkerColor(bgColor);
    totalBgHist->SetMarkerStyle(24);
    totalBgHist->SetMarkerSize(1.2);
    totalBgHist->SetLineWidth(3);
    totalBgHist->SetFillColor(0);

    // Set title and axis labels - focused on window analysis
    const std::string plotTitle = "Invariant Mass Window (" + CHANNEL_NAME + PROJ_SUFFIX + 
                                  ");Invariant Mass [GeV];Events";
    signalHist->SetTitle(plotTitle.c_str());
    signalHist->GetXaxis()->SetTitleSize(0.048);
    signalHist->GetYaxis()->SetTitleSize(0.048);
    signalHist->GetXaxis()->SetLabelSize(0.040);
    signalHist->GetYaxis()->SetLabelSize(0.040);
    signalHist->GetXaxis()->SetTitleOffset(1.05);
    signalHist->GetYaxis()->SetTitleOffset(1.10);
    signalHist->GetYaxis()->CenterTitle(true);

    // Find y-axis maximum considering both signal and background
    double maxY = signalHist->GetMaximum();
    if (totalBgHist->GetMaximum() > maxY)
        maxY = totalBgHist->GetMaximum();
    signalHist->SetMaximum(maxY * 1.35);
    signalHist->SetMinimum(0);
    
    // Set x-axis range to show full distribution without cutoff
    signalHist->GetXaxis()->SetRangeUser(0, 600);

    // Draw signal first
    signalHist->Draw("HIST");

    // Draw semi-transparent window box
    double lowEdge = TEST_MASS - WINDOW_HALFWIDTH;
    double highEdge = TEST_MASS + WINDOW_HALFWIDTH;
    TBox *windowBox = new TBox(lowEdge, 0, highEdge, maxY * 1.35);
    windowBox->SetFillColor(boxColor);
    windowBox->SetFillColorAlpha(boxColor, 0.3);  // 30% transparency
    windowBox->SetLineColor(kBlack);
    windowBox->SetLineWidth(1);
    windowBox->SetLineStyle(2);
    windowBox->Draw("SAME");

    // Draw background histogram on top
    totalBgHist->Draw("HIST SAME");

    // Redraw signal on top
    signalHist->Draw("HIST SAME");

    // Add text annotations - positioned in top margin below title
    TLatex label;
    label.SetNDC(true);
    label.SetTextSize(0.035);
    label.DrawLatex(0.13, 0.92, "CMSAnalysis Internal");
    
    label.SetTextSize(0.028);
    std::string windowLabel = "Window: " + std::to_string(TEST_MASS) + " #pm " + 
                              std::to_string((int)WINDOW_HALFWIDTH) + " GeV (" + 
                              std::to_string((int)lowEdge) + "-" + std::to_string((int)highEdge) + " GeV)";
    label.DrawLatex(0.13, 0.875, windowLabel.c_str());
    
    // Color key labels - positioned close together horizontally
    label.SetTextColor(signalColor);
    label.DrawLatex(0.50, 0.875, "Blue: Signal");
    label.SetTextColor(bgColor);
    label.DrawLatex(0.66, 0.875, "Yellow: Background");
    label.SetTextColor(kBlack);

    canvas->Modified();
    canvas->Update();

    // Force refresh of files
    gSystem->Unlink(OUTPUT_PNG.c_str());
    gSystem->Unlink(OUTPUT_PDF.c_str());

    // Save as PNG and PDF
    canvas->SaveAs(OUTPUT_PNG.c_str());
    canvas->Print(OUTPUT_PDF.c_str(), "pdf");

    std::cout << "Saved plots to " << OUTPUT_PNG << " and " << OUTPUT_PDF << "\n";

    // Clean up
    delete windowBox;
    delete canvas;
    delete totalBgHist;
}
