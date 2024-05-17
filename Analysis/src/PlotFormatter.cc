#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "TGraph.h"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TString.h"
#include "TPad.h"
#include "TFile.h"
#include "TFrame.h"
#include "TStyle.h"
#include "TPad.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TBox.h"
#include "TImage.h"
#include "TASImage.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"
#include "TFitResult.h"
#include <iostream>
#include <memory>
#include <vector>
#include <cmath> 

int width = 800;
int height = 600;
 
float top = 0.08*height;
float bottom = 0.12*height;
float left = 0.12*width;
float right = 0.04*width;

TCanvas* PlotFormatter::superImposedStackHist(std::shared_ptr<Channel> processes, std::string histvariable, TString xAxisTitle, TString yAxisTitle)
{
    THStack* background = processes->getStack(histvariable, "background", true);
	THStack* signal = processes->getStack(histvariable, "signal", true);

    //Defines order to draw in so graph isn't cut off
    THStack* first;
    THStack* second;
    if(signal->GetMaximum() > background->GetMaximum()) {
        first = signal;
        second = background;
    }
    else {
        first = background;
        second = signal;
    }
 
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    stackVector.push_back(first);

    auto hist = first->GetHistogram();
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
    std::string name;
    TString toAdd;
    for(const auto&& obj : *signal->GetHists()) {
        name = processes->getNamesWithLabel("signal").at(count); 
        toAdd = name;
        legend->AddEntry(obj, " " + toAdd, "L");
        count++;
    }
    count = 0;
    for(const auto&& obj2 : *background->GetHists()) {
        name = processes->getNamesWithLabel("background").at(count);
        toAdd = name;
        legend->AddEntry(obj2, " " + toAdd, "L");
        count++;
    }
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    second->Draw("HIST SAME");
    stackVector.push_back(second);

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::superImposedHist(std::shared_ptr<Channel> processes, std::string histvariable, TString xAxisTitle, TString yAxisTitle) {
    std::vector<TH1*> hists;
    for(std::shared_ptr<Process> process : processes->getProcesses()) {
        hists.push_back(process->getHist(histvariable, false));
    }

    std::vector<int> bins;
    for(TH1* hist : hists) {
        bins.push_back(hist->GetNbinsX());
    }

    int commonFactor = Utility::gcf(bins);

    for(TH1* hist : hists) {
        hist->Rebin(hist->GetNbinsX() / commonFactor);
    }

    double maxBinWidth = hists.at(0)->GetXaxis()->GetBinWidth(0);

    for(TH1* hist : hists) {
        if(hist->GetXaxis()->GetBinWidth(0) > maxBinWidth) {
            maxBinWidth = hist->GetXaxis()->GetBinWidth(0);
        }
    }
    for(TH1* hist : hists) {
        hist->Rebin((int) (maxBinWidth / hist->GetXaxis()->GetBinWidth(0)));
    }

    for(TH1* hist : hists) {
        if(hist->Integral() != 0 && !std::isnan(hist->Integral())) {
            hist->Scale(1/hist->Integral());
	    hist->SetFillColor(kWhite);
        }
    }

    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    for(TH1* hist : hists) {
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
    }
 
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
    std::string name;
    TString toAdd;
    for(TH1* hist : hists) {
        name = processes->getProcesses().at(count)->getName(); 
        toAdd = name;
        legend->AddEntry(hist, " " + toAdd, "L");
        count++;
    }
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
            hist->Draw("HIST SAME");
            histVector.push_back(hist);
        }
    }

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simpleAnalysisHist(std::vector<TH1*> hists, std::vector<std::string> names, TString xAxisTitle, TString yAxisTitle) {
    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    int count = 0;

    std::vector<int> bins;
    for(TH1* hist : hists) {
        bins.push_back(hist->GetNbinsX());
    }

    int commonFactor = Utility::gcf(bins);

    for(TH1* hist : hists) {
        hist->Rebin(hist->GetNbinsX() / commonFactor);
    }

    double maxBinWidth = hists.at(0)->GetXaxis()->GetBinWidth(0);

    for(TH1* hist : hists) {
        if(hist->GetXaxis()->GetBinWidth(0) > maxBinWidth) {
            maxBinWidth = hist->GetXaxis()->GetBinWidth(0);
        }
    }
    for(TH1* hist : hists) {
        hist->Rebin((int) (maxBinWidth / hist->GetXaxis()->GetBinWidth(0)));
    }

    for(TH1* hist : hists) {
	    if(hist->Integral() != 0 && !isnan(hist->Integral())) {
	        hist->Scale(1/hist->Integral());
		    hist->SetFillColor(kWhite);
	    }
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
	    count++;
    }
 
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    count = 0;
    std::string name;
    TString toAdd;
    for(TH1* hist : hists) {
        name = names.at(count); 
        toAdd = name;
        legend->AddEntry(hist, " " + toAdd, "L");
        count++;
    }
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
            hist->Draw("HIST SAME");
            histVector.push_back(hist);
        }
    }
    canvas->Update();
    return canvas;
}


TCanvas* PlotFormatter::simpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, TString xAxisTitle, TString yAxisTitle) 
{  
    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    int count = 0;

    std::vector<int> bins;
    for(TH1* hist : hists) {
        bins.push_back(hist->GetNbinsX());
    }

    int commonFactor = Utility::gcf(bins);

    for(TH1* hist : hists) {
        hist->Rebin(hist->GetNbinsX() / commonFactor);
    }

    double maxBinWidth = hists.at(0)->GetXaxis()->GetBinWidth(0);

    for(TH1* hist : hists) {
        if(hist->GetXaxis()->GetBinWidth(0) > maxBinWidth) {
            maxBinWidth = hist->GetXaxis()->GetBinWidth(0);
        }
    }
    for(TH1* hist : hists) {
        hist->Rebin((int) (maxBinWidth / hist->GetXaxis()->GetBinWidth(0)));
    }     

    for(TH1* hist : hists) {
	    if(hist->Integral() != 0 && !isnan(hist->Integral())) {
	        //hist->Scale(1/hist->Integral());
            //hist->GetYaxis()->SetLimits(0,0.7);
		    hist->SetFillColor(kWhite);
	    }
	    hist->SetLineColor(colors.at(count));
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
	    count++;
    }
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);
    gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    //first->GetYaxis()->ExtendAxis(0,0.7);
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);


    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    count = 0;
    std::string name;
    TString toAdd;
    for(TH1* hist : hists) {
        name = names.at(count); 
        toAdd = name;
        legend->AddEntry(hist, " " + toAdd, "L");
        count++;
    }
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
	    hist->Draw("HIST SAME");
            histVector.push_back(hist);
        }
    }

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::noScaleSimpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, TString xAxisTitle, TString yAxisTitle) {
    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    int count = 0;

    std::vector<int> bins;
    for(TH1* hist : hists) {
        bins.push_back(hist->GetNbinsX());
    }

    int commonFactor = Utility::gcf(bins);

    for(TH1* hist : hists) {
        hist->Rebin(hist->GetNbinsX() / commonFactor);
    }

    double maxBinWidth = hists.at(0)->GetXaxis()->GetBinWidth(0);

    for(TH1* hist : hists) {
        if(hist->GetXaxis()->GetBinWidth(0) > maxBinWidth) {
            maxBinWidth = hist->GetXaxis()->GetBinWidth(0);
        }
    }
    for(TH1* hist : hists) {
        hist->Rebin((int) (maxBinWidth / hist->GetXaxis()->GetBinWidth(0)));
    }     

    for(TH1* hist : hists) {
	    if(hist->Integral() != 0 && !isnan(hist->Integral())) {
		    hist->SetFillColor(kWhite);
	    }
	    hist->SetLineColor(colors.at(count));
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
	    count++;
    }

    int lowValue = maximum;
    for(TH1* hist : hists) {
        if(hist->GetMinimum() < lowValue) {
            lowValue = hist->GetMinimum();
        }
    }

    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis);
    first->SetMinimum(lowValue);
    //first->SetMinimum(-8);
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);


    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.015);
    legend->SetMargin(0.1);
    count = 0;
    std::string name;
    TString toAdd;
    for(TH1* hist : hists) {
        name = names.at(count); 
        toAdd = name;
        legend->AddEntry(hist, " " + toAdd, "L");
        count++;
    }
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
            // Double_t yminLimit = hist->GetMinimum();
            // Double_t ymaxLimit = hist->GetMaximum();
            // hist->GetYaxis()->SetRange(-10,10);
	        hist->Draw("HIST SAME");
            histVector.push_back(hist);
        }
    }

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simple1DHist(std::shared_ptr<Process> process, std::string histvariable, bool scaleToExpected, TString xAxisTitle, TString yAxisTitle) 
{
    TH1* hist = process->getHist(histvariable, scaleToExpected);
    return simple1DHist(hist, xAxisTitle, yAxisTitle);
}

TCanvas* PlotFormatter::simple1DHist(TH1* hist, TString xAxisTitle, TString yAxisTitle)
{
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram
    gStyle->SetOptStat(0);
    hist->Draw("HIST");
    //hist->SetMaximum(100);
    histVector.push_back(hist);
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);

    writeText(width, height, top, bottom, left, right);
   
    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simple2DHist(std::shared_ptr<Process> process, std::string histvariable, TString xAxisTitle, TString yAxisTitle) {
    TH2* hist = process->get2DHist(histvariable);
 
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram
    hist->Draw("COLZ");
    th2Vector.push_back(hist);
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);

    writeText(width, height, top, bottom, left, right);
   
    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simpleStackHist(std::shared_ptr<Channel> processes, std::string histvariable, TString xAxisTitle, TString yAxisTitle) {
    THStack* hists = processes->getStack(histvariable, "", true);
 
    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    hists->Draw("HIST");
    stackVector.push_back(hists);

    auto hist = hists->GetHistogram();
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
    std::string name;
    TString toAdd;
    for(const auto&& obj : *hists->GetHists()) {
        name = processes->getNames().at(count); 
        toAdd = name;
        legend->AddEntry(obj, " " + toAdd, "L");
        count++;
    }
    
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::completePlot(std::shared_ptr<FullAnalysis> analysis, std::string histvariable, TString xAxisTitle, TString yAxisTitle, double massTarget, bool scaleTodata, std::string channelName)
{
    std::shared_ptr<Channel> processes = 0;

    TH1* data;
    TH1* signal;
    THStack* background;
    // mass range
    // needs to match the "xAxisRange" value in "SimpleEstimator" if using integral scaling
    double upperMasslimit = 1200;

    //int firstBin = 0;
    int numBins = 8;
    
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    processes = channels.at(0);
    std::vector<std::string> backgroundNames = processes->getNamesWithLabel("background");
    std::vector<std::string> signalNames = processes->getNamesWithLabel("signal");
    std::vector<std::string> dataNames = processes->getNamesWithLabel("data");
    data = analysis->getHist(histvariable, dataNames.at(0), false, channelName);
    signal = analysis->getHist(histvariable, signalNames.at(0), true, channelName);
    std::vector<TH1*> backgroundHists;
    for(std::string name : backgroundNames) {
        backgroundHists.push_back(analysis->getHist(histvariable, name, true, channelName));
    }

//commented out integral code
// /*
    // integral/fit only calibrated after firstBin
    // needs to match the "firstBin" value in "SimpleEstimator" if using integral scaling
    int firstBin = 50;
    
    int numberBinsData = data->GetNbinsX();
    int lowerDataIntegralLimit = firstBin*(static_cast<double>(numberBinsData)/upperMasslimit);
    float dataIntegral = data->Integral(lowerDataIntegralLimit, numberBinsData);
    
    float backgroundIntegral = 0;
    for (auto backgroundHist : backgroundHists)
    {
        int numberBinsBackground = backgroundHist->GetNbinsX();
        int lowerBackgroundIntegralLimit = firstBin*(static_cast<double>(numberBinsBackground)/upperMasslimit);
        backgroundIntegral += backgroundHist->Integral(lowerBackgroundIntegralLimit, numberBinsBackground);
    }
    float scaleFactor = dataIntegral/backgroundIntegral;
    if (scaleTodata == true)
    {
        for (auto backgroundHist : backgroundHists)
        {
            backgroundHist->Scale(scaleFactor);
        }
    }
// */

    background = new THStack("background", "background");
    for(TH1* backgroundHist : backgroundHists) {
        backgroundHist->Rebin(numBins);
        background->Add(backgroundHist);
    }
    signal->Rebin(numBins);
    data->Rebin(numBins);   

    signal->SetLineColor(6);
	signal->SetFillColor(6);

    data->SetLineColor(kBlack);
    data->SetFillColor(kWhite);
    data->SetMarkerStyle(8);
    data->SetMarkerSize(0.5);

    //Defines order to draw in so graph isn't cut off
    int first;
    if(signal->GetMaximum() > background->GetMaximum() && signal->GetMaximum() > data->GetMaximum()) {
        first = 1;
    }
    else if(data->GetMaximum() > background->GetMaximum() && data->GetMaximum() > signal->GetMaximum()) {
        first = 2;
    }
    else {
        first = 0;
    }

    //Setting size and margins
    //int width = 800;
    //int height = 600;
 
    //float top = 0.08*height;
    //float bottom = 0.12*height;
    //float left = 0.12*width;
    //float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);
    TPad* topPad = new TPad("pad1", "", 0, 0.25, 1, 1);
    TPad* bottomPad = new TPad("pad2", "", 0, 0, 1, 0.25);
    topPad->SetLogy();
    gStyle->SetOptStat(0);

    topPad->Draw();
    topPad->cd();

    TH1* histLoop;
    //Draws the histogram with more events first (bigger axis)
    int xAxisMin = std::pow(1, -5);
    if(first == 0) {
        for(const auto&& obj2 : *background->GetHists()) {
            TH1* backgroundHist = dynamic_cast<TH1*>(obj2);
            backgroundHist->SetLineColor(kBlack);
            backgroundHist->SetLineWidth(2);
            backgroundHist->GetXaxis()->SetLimits(firstBin, upperMasslimit);
            backgroundHist->SetMinimum(xAxisMin);
        }
        background->Draw("HIST");
        stackVector.push_back(background);  
    }
    else if(first == 1) {
        signal->SetLineColor(kBlack);
        signal->SetLineWidth(2);
        signal->SetMinimum(xAxisMin);
        //signal->Draw("HIST");
        histVector.push_back(signal);
        signal->GetXaxis()->SetLimits(firstBin, upperMasslimit);
    }
    else {
        data->Draw("P E1 X0");
        data->SetMinimum(xAxisMin);
        histVector.push_back(data);
        data->GetXaxis()->SetLimits(firstBin, upperMasslimit);
    }

    TH1* hist;
    if(first == 0) {
        std::cout << "check 1";
        hist = background->GetHistogram();
    }
    else if (first == 1){
        hist = signal;
    }
    else {
        hist = data;
    }
    topPad->Update();
    
    //Change axis and graph titles & sizes here
    hist->GetYaxis()->SetTitle(yAxisTitle);
    hist->SetTitleSize(0.04, "y");
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->SetTitleSize(0.04, "x");
    
    //Draws the legend
    /*
    auto legend = new TLegend(0.55 - (right/width), 0.75 - (top/height), 1 - (right/width), 1 - (top/height));
    legend->SetTextSize(0.04);
    std::string name;
    TString toAdd;
    name = processes->getNamesWithLabel("data").at(0); 
    toAdd = name;
    legend->AddEntry(data, " " + toAdd, "L");
    //name = processes->getNamesWithLabel("signal").at(0); 
    //toAdd = name;
    //legend->AddEntry(signal, " " + toAdd, "F");
    int count = 0;
    for(const auto&& obj2 : *background->GetHists()) {
        std::cout << "count";
        name = processes->getNamesWithLabel("background").at(count);
        toAdd = name;
        legend->AddEntry(obj2, " " + toAdd, "F");
        count++;
    }
    */
    auto legend = GetLegend(background, processes, data);
    legend->Draw();
    topPad->Update();
    writeText(width, height, top, bottom, left, right);

    //Draws the other histogram    
    if(first == 0) {
        //signal->Draw("HIST SAME");
        std::cout << "first = 0";
        signal->SetLineColor(kBlack);
        signal->SetLineWidth(2);
        histVector.push_back(signal);
        data->Draw("P SAME E1 X0");
        data->SetLineColor(kBlack);
        data->SetLineWidth(2);
        histVector.push_back(data);
    }
    else if(first == 1) {
        std::cout << "first = 1";
        //background->SetLineColor(kBlack);
        background->Draw("HIST SAME");
        stackVector.push_back(background);
        data->Draw("P SAME E1 X0");
        data->SetLineColor(kBlack);
        data->SetLineWidth(2);
        histVector.push_back(data);
    }
    else {
        std::cout << "first = 2";
        //signal->Draw("HIST SAME");
        signal->SetLineColor(kBlack);
        signal->SetLineWidth(2); 
        histVector.push_back(signal);
        background->Draw("HIST SAME");
        //background->SetLineColor(kBlack);
        stackVector.push_back(background);
        data->Draw("P SAME E1 X0");
    }
   
    /*
    //for signal errors
    auto errorgraph = new TGraphErrors(signal->GetNbinsX(), x1, y1, xerror, yerror);
    TAxis *eaxis = errorgraph->GetXaxis();
    eaxis->SetLimits(0, signal->GetNbinsX() * signal->GetBinWidth(signal->GetNbinsX()));
    errorgraph->SetFillStyle(3004);
    errorgraph->SetFillColor(kBlack);
    //errorgraph->Draw("P E3 SAME");
    errorgraph->Draw("P E0 SAME X0");
    */
    
    //Draws on bottom pad
    topPad->Update();
    topPad->Modified();

    canvas->cd();

    bottomPad->Draw();
    bottomPad->cd();

    double x[data->GetNbinsX()];
    double y[data->GetNbinsX()];
    double xerror2[data->GetNbinsX()];
    double yerror2[data->GetNbinsX()];
    GetBottomPad(x, y, xerror2, yerror2);
    /*
    double backgroundHistBinMax = 0;
    double value;
    for(int i = 0; i < data->GetNbinsX(); i++) {
        double total = 0;
        x[i] = i * data->GetBinWidth(0);
        for(const auto&& obj : *(background->GetHists())) { //How you iterate over a TList
            histLoop = dynamic_cast<TH1*>(obj);
            total+= histLoop->GetBinContent(i);
        }
        xerror2[i] = 0;
        if(total != 0) {
            y[i] = (data->GetBinContent(i) - total) / total;
            backgroundHistBinMax = 0;
            for(const auto&& obj2 : *background->GetHists()) {
                TH1* backgroundHist = dynamic_cast<TH1*>(obj2);
                value = backgroundHist->GetBinContent(i);
                if (value > backgroundHistBinMax)
                {
                    backgroundHistBinMax = value;
                }
            }
            if (backgroundHistBinMax != 0)
            {
                yerror2[i] = (data->GetBinError(i) / backgroundHistBinMax);
            }
            else
            {
                yerror2[i] = 0;
            }
        }
        else {
            y[i] = 0;
            yerror2[i] = 0;
        }
    }
    */

    auto graph = new TGraph(data->GetNbinsX(), x, y);
    GraphFormat(graph, xAxisTitle, 0.08, 0.04, 0.06, 0.07, 0.5, 1, -1);
    graph->Draw("AP SAME");

    auto errorgraph2 = new TGraphErrors(data->GetNbinsX(), x, y, xerror2, yerror2);
    errorgraph2->SetFillColor(16);
    errorgraph2->Draw("P SAME E0");
    /*
    auto graph = new TGraph(data->GetNbinsX(), x, y);
    //sets an empty global title; xAxis' title; yAxis' title
    graph->SetTitle(";" + xAxisTitle +";(Data - Bkg) / Bkg");
    graph->GetXaxis()->SetLabelSize(0.08);
    graph->GetXaxis()->SetTitleSize(0.04);
    graph->GetYaxis()->SetLabelSize(0.06);
    graph->GetYaxis()->SetTitleSize(0.07);
    graph->SetMarkerSize(0.5);
    graph->SetMarkerStyle(kFullDotLarge);
    graph->SetMaximum(1);   
    graph->SetMinimum(-1);
    */
    
    //TAxis *axis = graph->GetXaxis();
    //axis->SetLimits(0, data->GetNbinsX() * data->GetBinWidth(data->GetNbinsX()));
    //axis->SetLimits(firstBin, upperMasslimit);
    //graph->Draw("AP SAME");
    //errorgraph2->SetFillColor(16);
    //errorgraph2->Draw("P SAME E3");
    //errorgraph2->Draw("P SAME E0");

    bottomPad->Update();
    bottomPad->Modified();

    canvas->Update();
    return canvas;
}

void GetBottomPadValues(ref double[] x, ref double[] y, ref double[] xerror2, ref double[] yerror2)
{
    double backgroundHistBinMax = 0;
    double value;
    for(int i = 0; i < data->GetNbinsX(); i++) {
        double total = 0;
        x[i] = i * data->GetBinWidth(0);
        for(const auto&& obj : *(background->GetHists())) { //How you iterate over a TList
            histLoop = dynamic_cast<TH1*>(obj);
            total+= histLoop->GetBinContent(i);
        }
        xerror2[i] = 0;
        if(total != 0) {
            y[i] = (data->GetBinContent(i) - total) / total;
            backgroundHistBinMax = 0;
            for(const auto&& obj2 : *background->GetHists()) {
                TH1* backgroundHist = dynamic_cast<TH1*>(obj2);
                value = backgroundHist->GetBinContent(i);
                if (value > backgroundHistBinMax)
                {
                    backgroundHistBinMax = value;
                }
            }
            if (backgroundHistBinMax != 0)
            {
                yerror2[i] = (data->GetBinError(i) / backgroundHistBinMax);
            }
            else
            {
                yerror2[i] = 0;
            }
        }
        else {
            y[i] = 0;
            yerror2[i] = 0;
        }
    }
}

void PlotFormatter::GraphFormat(TGraph*& graph, TString xAxisTitle, float xLabelSize, float xTitleSize, float yLableSize, 
float yTitleSize, float markerSize, float maximum, float minimum, float firstBin, float upperMassLimit)
{
    graph->SetTitle(";" + xAxisTitle +";(Data - Bkg) / Bkg");
    graph->GetXaxis()->SetLabelSize(xLabelSize);
    graph->GetXaxis()->SetTitleSize(xTitleSize);
    graph->GetYaxis()->SetLabelSize(yLableSize);
    graph->GetYaxis()->SetTitleSize(yTitleSize);
    graph->SetMarkerSize(markerSize);
    graph->SetMarkerStyle(kFullDotLarge);
    graph->SetMaximum(maximum);   
    graph->SetMinimum(minimum);
    TAxis *axis = graph->GetXaxis();
    axis->SetLimits(firstBin, upperMasslimit);
}

TCanvas* PlotFormatter::makeFormat(int w, int h, float t, float b, float l, float r) {
    TString canvasName = "Canvas";
    canvasName += w;
    canvasName += "_";
    canvasName += h;
 
    gStyle->SetOptTitle(0);

    //Creates and formats canvas
    TCanvas* canvas = new TCanvas(canvasName, canvasName, 50, 50, w, h);
    canvas->SetFillColor(0);
    canvas->SetBorderMode(0);
    canvas->SetFrameFillStyle(0);
    canvas->SetFrameBorderMode(0);
    canvas->SetLeftMargin(l/w);
    canvas->SetRightMargin(r/w);
    canvas->SetTopMargin(t/h);
    canvas->SetBottomMargin(b/h);
    canvas->SetTickx(0);
    canvas->SetTicky(0);

    return canvas;
}

void PlotFormatter::writeText(int w, int h, float t, float b, float l, float r) {
    //Writes CMS logo and integrated luminosity
    int align_ = 13;
    TString lumiText = "13 TeV";
    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);    
    latex.SetTextFont(42);
    latex.SetTextAlign(31);
    latex.SetTextSize(0.045);
    latex.DrawLatex(1-(r/w), 1-(t/h) + 0.025, lumiText);
 
    float posX_ = (l/w) + 0.05;
    float posY_ = 0.95 - (t/h);
 
    if(drawLogo) {
        //This code doesn't compile for some reason
        // float xl_0 = posX_;
        // float yl_0 = posY_ - 0.15;
        // float xl_1 = posX_ + 0.15*height/width;
        // float yl_1 = posY_;
        // TASImage* CMS_logo = new TASImage("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/cmslogo.png");
        // TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
        // pad_logo->Draw();
        // pad_logo->cd();
        // CMS_logo->Draw("X");
        // pad_logo->Modified();
        // canvas->cd();
    }
    else {
        latex.SetTextFont(61);
        latex.SetTextSize(0.055);
        latex.SetTextAlign(align_);
        latex.DrawLatex(posX_, posY_, "CMS");
    }
    if(extraText != "") {
        latex.SetTextFont(52);
        latex.SetTextAlign(align_);
        latex.SetTextSize(0.04);
        latex.DrawLatex(posX_, posY_ - 0.05, extraText);
    }
}

void PlotFormatter::deleteHists() {
    for(auto hist : histVector) {
        delete hist;
    }
    for(auto stack : stackVector) {
	TIter next(stack->GetHists());
	TObject* object = 0;
	while ((object = next())) {
	    delete (TH1*)object;
	}		
        delete stack;

    }
    for(auto hist2 : th2Vector) {
	delete hist2;
    }
    histVector.clear();
    stackVector.clear();
    th2Vector.clear();
}

TLegend* PlotFormatter::GetLegend(THStack* background, std::shared_ptr<Channel> processes, TH1* data)
{
    //Draws the legend
    auto legend = new TLegend(0.55 - (right/width), 0.75 - (top/height), 1 - (right/width), 1 - (top/height));
    legend->SetTextSize(0.04);
    std::string name;
    TString toAdd;
    name = processes->getNamesWithLabel("data").at(0); 
    toAdd = name;
    legend->AddEntry(data, " " + toAdd, "L");
    //name = processes->getNamesWithLabel("signal").at(0); 
    //toAdd = name;
    //legend->AddEntry(signal, " " + toAdd, "F");
    int count = 0;
    for(const auto&& obj2 : *background->GetHists()) {
        std::cout << "count";
        name = processes->getNamesWithLabel("background").at(count);
        toAdd = name;
        legend->AddEntry(obj2, " " + toAdd, "F");
        count++;
    }
    return legend;
}

/*
TLegend* PlotFormatter::GetLegend(std::vector<TH1*> hists, std::vector<TString> names)
{
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    count = 0;
    std::string name;
    TString toAdd;
    for(TH1* hist : hists) {
        name = names.at(count); 
        toAdd = name;
        legend->AddEntry(hist, " " + toAdd, "L");
        count++;
    }
    return legend;
}
*/


