#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/MultiSystematic.hh"
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

TCanvas* PlotFormatter::superImposedStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, TString xAxisTitle, TString yAxisTitle)
{
    THStack* background = processes->getStack(histvariable, "background", true);
	THStack* signal = processes->getStack(histvariable, "signal", true);

    //Defines order to draw in so graph isn't cut off
    THStack* first;
    THStack* second;
    GetImposedOrder(background, signal, first, second);

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    stackVector.push_back(first);

    auto hist = first->GetHistogram();
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = GetSuperImposedLegend(processes, histvariable);
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    second->Draw("HIST SAME");
    stackVector.push_back(second);

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::superImposedHist(std::shared_ptr<Channel> processes, HistVariable histvariable, TString xAxisTitle, TString yAxisTitle) {
    std::vector<TH1*> hists;
    for(std::shared_ptr<Process> process : processes->getProcesses()) {
        hists.push_back(process->getHist(histvariable, false));
    }

    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    int count = 0;
    Bin(hists, first, firstIndex, maximum, count, true);
    GetOrder(hists, first, firstIndex, maximum);

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);
    
    std::vector<std::string> names;
    count = 0;
    for (TH1* hist : hists)
    {
        names.push_back(processes->getProcesses().at(count)->getName());
        count++;
    }
    auto legend = GetSimpleLegend(hists, names);
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram  
    DrawOtherHistograms(hists, firstIndex);

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simpleAnalysisHist(std::vector<TH1*> hists, std::vector<std::string> names, TString xAxisTitle, TString yAxisTitle) {
    //Defines order to draw so graph isn't cut off
    TH1* first = hists.at(0);
    int firstIndex = 0;
    double maximum = 0;
    int count = 0;

    Bin(hists, first, firstIndex, maximum, count, false);

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
    histVector.push_back(first);

    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = GetSimpleLegend(hists, names);
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    DrawOtherHistograms(hists, firstIndex);

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

    Bin(hists, first, firstIndex, maximum, count, true);

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);
    gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->SetLineColor(colors.at(firstIndex));
    first->Draw("HIST");
    histVector.push_back(first);


    //Change axis and graph titles here
    first->GetXaxis()->SetTitle(xAxisTitle);
    first->GetYaxis()->SetTitle(yAxisTitle);

    //Draws the legend
    auto legend = GetSimpleLegend(hists, names);
    legend->Draw();

 
    writeText(width, height, top, bottom, left, right);
   
    //Draws the other histogram    
    DrawOtherHistograms(hists, colors, firstIndex);

    canvas->Update();
    return canvas;
}

    TCanvas* PlotFormatter::noScaleSimpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, TString xAxisTitle, TString yAxisTitle) 
    {
        //Defines order to draw so graph isn't cut off
        TH1* first = hists.at(0);
        int firstIndex = 0;
        double maximum = 0;
        int count = 0;

        Bin(hists, first, firstIndex, maximum, count, false);
        count=0;

        int lowValue = maximum;
        for(TH1* hist : hists) {
            if(hist->GetMinimum() < lowValue) {
                lowValue = hist->GetMinimum();
            }
            hist->SetLineColor(colors.at(count));
            count++;
        }

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
        auto legend = GetSimpleLegend(hists, names);
        legend->SetTextSize(0.015);
        legend->SetMargin(0.1);
        legend->Draw();
    
        writeText(width, height, top, bottom, left, right);
    
        //Draws the other histogram    
        DrawOtherHistograms(hists, firstIndex);

        canvas->Update();
        return canvas;
}

TCanvas* PlotFormatter::simple1DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool scaleToExpected, TString xAxisTitle, TString yAxisTitle) 
{
    TH1* hist = process->getHist(histvariable, scaleToExpected);
    return simple1DHist(hist, xAxisTitle, yAxisTitle);
}

TCanvas* PlotFormatter::simple1DHist(TH1* hist, TString xAxisTitle, TString yAxisTitle)
{
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

TCanvas* PlotFormatter::simple2DHist(std::shared_ptr<Process> process, HistVariable histvariable, TString xAxisTitle, TString yAxisTitle) {
    TH2* hist = process->get2DHist(histvariable);

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

TCanvas* PlotFormatter::simpleStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, TString xAxisTitle, TString yAxisTitle) {
    THStack* hists = processes->getStack(histvariable, "", true);

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    hists->Draw("HIST");
    stackVector.push_back(hists);

    auto hist = hists->GetHistogram();
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);
 
    //Draws the legend
    auto legend = GetStackedLegend(processes, hists);
    legend->Draw();
 
    writeText(width, height, top, bottom, left, right);

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::completePlot(std::shared_ptr<FullAnalysis> analysis, HistVariable histvariable, TString xAxisTitle, TString yAxisTitle, 
bool scaleTodata, bool includeSignal, bool includeData, std::string channelName)
{
    std::shared_ptr<Channel> processes = 0;

    TH1* data;
    TH1* signal;
    THStack* background;
    // mass range
    // needs to match the "xAxisRange" value in "SimpleEstimator" if using integral scaling
    //double upperMasslimit = 1200;

    //double upperMasslimit = 2000;

    //int firstBin = 0;
    
    //int numBins = 5;
    
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    processes = channels.at(0);

    /*
    for (auto channel : channels)
    {
        std::cout << channel->getName() << std::endl;
        for (auto process : channel->getNames())
        {
            std::cout << "Process: " << process << std::endl;
        }
    }
    */
   
    std::vector<std::string> backgroundNames = processes->getNamesWithLabel("background");
    //std::cout << "Background Names Size: " << backgroundNames.size() << std::endl;
    for (int i = 0; i < (int)backgroundNames.size(); i++)
    {
        //std::cout << backgroundNames[i] << std::endl;
    }
    std::vector<std::string> signalNames = processes->getNamesWithLabel("signal");

    std::vector<std::string> dataNames = processes->getNamesWithLabel("data");

    if (includeData == true)
    {
        data = analysis->getHist(histvariable, dataNames.at(0), false, channelName);
    }
    else
    {
        data = new TH1F("h1", "empty", 1, 0.0, 0.0);
    }

    std::cout << "Data has: " << data->GetEntries() << std::endl;

    //data = signal = new TH1F("h1", "empty", 1, 0.0, 0.0);
    if (includeSignal)
    {
        for(std::string name : signalNames) 
        {
            std::cout << histvariable.getName() << std::endl;
            std::cout << channelName << std::endl;
            std::cout << name << std::endl;
            signal = analysis->getHist(histvariable, name, true, channelName);
            std::cout << "number of signal bins is: " << signal->GetNbinsX();
            std::cout << name << std::endl;
            std::cout << histvariable.getName() << std::endl;
            std::cout << "SIGNAl MAX" << signal->GetMaximum() << std::endl;
        }
        //signal = analysis->getHist(histvariable, "Higgs Group 1000", true, channelName);
        //std::cout << "number of signal bins is: " << signal->GetNbinsX();
    }
    else
    {
        signal = new TH1F("h1", "empty", 1, 0.0, 0.0);
    }
    //std::cout << "HERE" << std::endl;
    double maxCombinedY = signal->GetMaximum();

    std::vector<TH1*> backgroundHists;
   
    std::vector<std::shared_ptr<Process>> backgroundProcesses;
    for(std::string name : backgroundNames)
    {
        //std::cout << name << std::endl;
        //std::cout << channelName << std::endl;
        //std::cout << histvariable.getName() << std::endl;
        
        auto hist = analysis->getHist(histvariable, name, true, channelName);
        
        backgroundProcesses.push_back(analysis->getChannel(channelName)->findProcess(name));
        //auto hist = process->getSystematicHist(histvariable, true).second;

        // TEST
        if (!hist)
        {
            continue;
        }
        //std::cout << hist->GetName() << " has "<< hist->GetNbinsX() << std::endl;
        backgroundHists.push_back(hist);
        
        maxCombinedY += hist->GetMaximum();
    }
    std::cout << backgroundHists.size() << "\n";
    //std::cout << "End" << std::endl;

    //int firstBin = 50;
    int numberBinsData = data->GetNbinsX();
    int lowerDataIntegralLimit = firstBin*(static_cast<double>(numberBinsData)/upperMasslimit);
    float dataIntegral = data->Integral(lowerDataIntegralLimit, numberBinsData);
    float backgroundIntegral = 0;

    IntegralScaling(upperMasslimit, scaleTodata, backgroundHists, firstBin, numberBinsData,
        lowerDataIntegralLimit, dataIntegral, backgroundIntegral);
    
    background = new THStack("background", "background");

    maxCombinedY *= 100;
    data->SetMaximum(maxCombinedY );
    signal->SetMaximum(maxCombinedY);
    background->SetMaximum(maxCombinedY);
    //std::cout << "MAXCOMBINEDY: " << maxCombinedY << std::endl;
    
    FormatSignalData(background, signal, data, backgroundHists, numBins);

    //std::cout << "1" << std::endl;

    //Defines order to draw in so graph isn't cut off
    int first = 0;
    //GetOrder(data, signal, background);

    //std::cout << "1.1" << std::endl;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);
    //std::cout << "1.2" << std::endl;
    TPad* topPad = new TPad("pad1", "", 0, 0.25, 1, 1);
    TPad* bottomPad = new TPad("pad2", "", 0, 0, 1, 0.25);
    topPad->SetLogy();
    gStyle->SetOptStat(0);
    topPad->Draw();
    topPad->cd();
    //std::cout << "1.3" << std::endl;


    //Draws the histogram with more events first (bigger axis)
    //TH1* hist = DrawFirst(background, signal, data, topPad, upperMasslimit, firstBin, first);
    TH1* hist = DrawFirst(background, signal, data, topPad, upperMasslimit, firstBin, first);

    hist->SetMinimum(1e-2);

    //std::cout << "1.4" << std::endl;


    ChangeAxisTitles(hist, xAxisTitle, yAxisTitle);

    //std::cout << "2" << std::endl;
    
    //Draws the legend
    auto legend = GetLegend(background, processes, data, signal, includeSignal, includeData);
    legend->Draw();
    topPad->Update();

    writeText(width, height, top, bottom, left, right);

    //Draws the other histogram   
    DrawOtherHistograms(background, signal, data, first); 
    std::cout <<"before creating histogram" << std::endl;
    auto backgroundHist = CreateErrorHistogram(background,  backgroundProcesses);
    backgroundHist->SetFillColor(kBlack);
    backgroundHist->SetFillStyle(3018);
    // backgroundHist->SetLineColor(kRed);
    // backgroundHist->SetLineWidth(5);
    backgroundHist->Draw("E2 SAME");
    std::cout <<"Integral: " << backgroundHist->Integral() << std::endl;
//    backgroundHist->Draw("E2 SAME");
    
    //Draws on bottom pad
    topPad->Update();
    topPad->Modified();
    canvas->cd();
    bottomPad->Draw();
    bottomPad->cd();


    
    std::vector<double> x(data->GetNbinsX() + 1);
    std::vector<double> y(data->GetNbinsX() + 1);
    std::vector<double> xerror2(data->GetNbinsX() + 1);
    std::vector<double> yerror2(data->GetNbinsX() + 1);

   // std::cout << "3" << std::endl;
    std::vector<double> centers;
    GetBottomPadValues(data, background, backgroundHist, x, y, xerror2, yerror2, centers);
    auto graph = new TGraph(data->GetNbinsX() + 1, x.data(), y.data());
    //auto graph = new TGraph();

    // auto zeros = new std::vector<double>(data->GetNbinsX() + 1, 0.0);

    auto errorgraph2 = new TGraphErrors(data->GetNbinsX() + 1, x.data(), centers.data(), xerror2.data(), yerror2.data());
    //auto errorgraph2 = new TGraphErrors();
    GraphFormat(graph, errorgraph2, xAxisTitle, 0.08, 0.04, 0.06, 0.07, 0.5, 1, -1, firstBin, upperMasslimit);

    TAxis *axis = graph->GetXaxis();
    //axis->SetLimits(firstBin, hist->GetXaxis()->GetXmax());
    axis->SetLimits(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    //graph->SetMaximum(10);
    //std::cout << "4" << std::endl;

    graph->Draw("AP SAME");
    errorgraph2->GetXaxis()->SetLimits(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    errorgraph2->SetFillColor(16);
    errorgraph2->Draw("SAME E3 0");

    // auto firstHist = dynamic_cast<TH1*>(background->GetHists()->At(0));
    // auto process = backgroundHists
    // firstHist->getSystematicHist(histvariable, true)->Draw("E5");

    //->getSystematicHist(histvariable,true).second->Draw("E5");
    //auto process = analysis->getChannel(channelName)->findProcess(name);
        //auto hist = process->getSystematicHist(histvariable, true).second;

    bottomPad->Update();
    bottomPad->Modified();

    canvas->Update();
    return canvas;
}

void PlotFormatter::FormatSignalData(THStack*& background, TH1*& signal, TH1*& data, std::vector<TH1*>& backgroundHists, int numBins)
{
    //std::cout << "Background Hist Names Start" << std::endl;
    for(TH1* backgroundHist : backgroundHists) {
        backgroundHist->Rebin(numBins);
        background->Add(backgroundHist);
        //std::cout << backgroundHist->GetName() << std::endl;
    }
    //std::cout << "Background Hist Names End" << std::endl;

    //std::cout << "TList Start" << std::endl;
    TList *histList = background->GetHists();
    TIter next(histList);  
    TH1 *hist;  
    bool emptyHist = false;
    while ((hist = (TH1*)next())) {
    //std::cout << hist->GetName() << " entries " << hist->GetEntries() << std::endl;  // Print info about each histogram
    if (hist->GetEntries() == 0) { continue; }
    }
    //std::cout << "TList End" << std::endl;

    //signal->Scale(std::pow(10, 6));
    signal->Rebin(numBins);
    data->Rebin(numBins);   

    signal->SetLineColor(6);
	signal->SetFillColor(6);
    signal->SetMarkerStyle(8);
    signal->SetMarkerSize(0.5);

    data->SetLineColor(kBlack);
    data->SetFillColor(kWhite);
    data->SetMarkerStyle(8);
    data->SetMarkerSize(0.5);
}

void PlotFormatter::ChangeAxisTitles(TH1*& hist, TString xAxisTitle, TString yAxisTitle)
{
    hist->GetYaxis()->SetTitle(yAxisTitle);
    hist->SetTitleSize(0.04, "y");
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->SetTitleSize(0.04, "x");
}

void PlotFormatter::DrawOtherHistograms(std::vector<TH1*>& hists, std::vector<int>& colors, int firstIndex)
{
    for (size_t i = 0; i < hists.size(); ++i) {
        if (static_cast<int>(i) == firstIndex) continue; // Skip the first histogram, already drawn

        TH1* hist = hists[i];
        hist->SetLineColor(colors[i]);
        hist->SetLineWidth(2);
        hist->Draw("HIST SAME");
        histVector.push_back(hist);

        std::cout << "First index: " << firstIndex << "Index " << i << " Hist name " << hist->GetName() << " color " << hist->GetLineColor() << std::endl;
    }
}

void PlotFormatter::DrawOtherHistograms(THStack*& background, TH1*& signal, TH1*& data, int first)
{
    if(first == 0) {
        signal->SetLineColor(6);
	    signal->SetFillColor(0);
        signal->SetLineWidth(3);
        signal->Draw("HIST SAME");
        std::cout << "first = 0";
        std::cout << "Hist content: " << signal->Integral() << '\n';
        //signal->SetLineColor(kBlack);
        //signal->SetLineWidth(2);
        histVector.push_back(signal);
        data->Draw("P SAME E1 X0");
        data->SetLineColor(kBlack);
        data->SetLineWidth(2);
        histVector.push_back(data);
    }
    else if(first == 1) 
    {
        std::cout << "first = 1";
        //background->SetLineColor(kBlack);
        background->Draw("HIST SAME E5");
        stackVector.push_back(background);
        data->Draw("P SAME E1 X0");
        data->SetLineColor(kBlack);
        data->SetLineWidth(2);
        histVector.push_back(data);
    }
    else 
    {
        std::cout << "first = 2";
        signal->Draw("HIST SAME");
        signal->SetLineColor(kBlack);
        signal->SetLineWidth(2);
        histVector.push_back(signal);
        background->Draw("HIST SAME");
        //std::cout<<"hist size" << background->Integral();
        //background->SetLineColor(kBlack);
        stackVector.push_back(background);
        for (int i = 0; i < data->GetNbinsX(); i++)
        {
            data->SetBinError(i, 0);
        }
        data->Draw("P SAME E1 X0");
    }

}

TH1* PlotFormatter::DrawFirst(THStack*& background, TH1*& signal, TH1*& data, TPad*& topPad, float upperMasslimit, int firstBin, int first)
{
    TH1* hist;
    //std::cout << "1.31" << std::endl;
    hist = background->GetHistogram();
    double xAxisMin = 1e-5;
    std::cout << background->GetName() << std::endl;
    std::cout << background->GetNhists() << std::endl;
    if(first == 0) {
        for(const auto&& obj2 : *background->GetHists()) {
            TH1* backgroundHist = dynamic_cast<TH1*>(obj2);
            //backgroundHist->SetLineColor(kBlack);
            backgroundHist->SetLineWidth(2);
           // backgroundHist->GetXaxis()->SetLimits(firstBin, upperMasslimit);
            backgroundHist->SetMinimum(xAxisMin);
        }
        background->Draw("HIST");
        stackVector.push_back(background);  
    }
    else if (first == 1) 
    {
        //signal->SetLineColor(kBlack);
        signal->SetLineColor(6);
	    signal->SetFillColor(0);
        signal->SetLineWidth(2);
        signal->SetMinimum(xAxisMin);
        signal->Draw("HIST");
        signal->Draw("HIST");
        histVector.push_back(signal);
       // signal->GetXaxis()->SetLimits(firstBin, upperMasslimit);
    }
    else {
        data->Draw("P E1 X0");
        data->SetMinimum(xAxisMin);
        histVector.push_back(data);
        //data->GetXaxis()->SetLimits(firstBin, upperMasslimit);
    }
    //std::cout << "1.32" << std::endl;

    //TH1* hist;
    hist = background->GetHistogram();

    std::cout << first << std::endl;
    if(first == 0) {
        //std::cout << "check 1" << "\n";
        hist = background->GetHistogram();
    }
    else if (first == 1){
        hist = signal;
    }
    else {
        hist = data;
    }
    //std::cout << "1.33" << std::endl;
    topPad->Update();
    return hist;
}

int PlotFormatter::GetOrder(TH1* data, TH1* signal, THStack* background)
{
    int first;
    double dataMax =  data->GetMaximum();
    double signalMax = signal->GetMaximum();
    double backgroundMax = background->GetMaximum();

    if(data->GetEntries() == 0)
    {
        dataMax=0;
        std::cout << "No Data Entries" << std::endl;
    }

    if(signalMax > backgroundMax && signalMax > dataMax)
    {
        first=1;
    }
    else if(dataMax > backgroundMax && dataMax > signalMax)
    {
        first=2;
    }
    else
    {
        first=0;
    }
    
    return first;
}

void PlotFormatter::IntegralScaling(double& upperMasslimit, bool& scaleTodata, std::vector<TH1*>& backgroundHists, int& firstBin, int& numberBinsData, int& lowerDataIntegralLimit, float& dataIntegral, float& backgroundIntegral)
{
    for (auto backgroundHist : backgroundHists)
    {
        int numberBinsBackground = backgroundHist->GetNbinsX();
        int lowerBackgroundIntegralLimit = firstBin*(static_cast<double>(numberBinsBackground)/upperMasslimit);
        backgroundIntegral += backgroundHist->Integral(lowerBackgroundIntegralLimit, numberBinsBackground);
    }
  
    float scaleFactor = dataIntegral/backgroundIntegral;

    if(dataIntegral == 0)
    {
        scaleFactor=1;
        std::cout << "No Data Integral" << std::endl;
    } 

    if (scaleTodata)
    {
        for (auto backgroundHist : backgroundHists)
        {
            backgroundHist->Scale(scaleFactor);
        }
    }
}


void PlotFormatter::GetBottomPadValues(TH1*& data, THStack*& background, TH1* backgroundHist, 
    std::vector<double>& x,  std::vector<double>& y,  std::vector<double>& xerror2,
    std::vector<double>& yerror2, std::vector<double>& centers)
{
    TH1* histLoop;
    double value;

    for(int i = 0; i <= data->GetNbinsX(); i++) 
    {
        double total = 0;
        //x[i] = i * data->GetBinWidth(0);
        x[i] = data->GetBinCenter(i);
        for(const auto&& obj : *(background->GetHists()))
        { //How you iterate over a TList
            histLoop = dynamic_cast<TH1*>(obj);
            total+= histLoop->GetBinContent(i);
        }

        if (backgroundHist->GetBinContent(i) == 0)
        {
            centers.push_back(0);
        }
        else
        {
            centers.push_back((data->GetBinContent(i) - backgroundHist->GetBinContent(i)) / backgroundHist->GetBinContent(i));
        }
        xerror2[i] = 0;
        if(total != 0) 
        {
            y[i] = (data->GetBinContent(i) - total) / total;

            if (backgroundHist->GetBinContent(i) != 0)
            {
                std::cout << "Bin: " << i << " Background: " << backgroundHist->GetBinContent(i) << "  error: " << backgroundHist->GetBinError(i) << std::endl;
                yerror2[i] = (backgroundHist->GetBinError(i) / total);
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
        std::cout << "x: " << x[i] << " y: " << y[i] << " yerror2: " << yerror2[i] << " center: " << centers[i] << std::endl;
    }
}


void PlotFormatter::GraphFormat(TGraph*& graph, TGraphErrors*& errorgraph2, TString xAxisTitle, float xLabelSize, float xTitleSize, float yLableSize, 
float yTitleSize, float markerSize, float maximum, float minimum, float firstBin, float upperMasslimit)
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

void PlotFormatter::writeText(int w, int h, float t, float b, float l, float r) 
{
    //Writes CMS logo and integrated luminosity
    int align_ = 13; 
    TString lumiText = "139 f^-1, 13 TeV";
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

void PlotFormatter::deleteHists() 
{
    for(auto hist : histVector) 
    {
        std::cout << "Deleting " << hist->GetName() <<std::endl;
        delete hist;
    }
    for(auto stack : stackVector) 
    {
	    TIter next(stack->GetHists());
	    TObject* object = nullptr;
	    while ((object = next())) 
        {
            std::cout << "Deleting " << ((TH1*)object)->GetName() <<std::endl;
	        delete object;
	    }		
        delete stack;

    }
    for(auto hist2 : th2Vector) 
    {
        std::cout << "Deleting " << hist2->GetName() <<std::endl;
	    delete hist2;
    }
    histVector.clear();
    stackVector.clear();
    th2Vector.clear();
}

TLegend* PlotFormatter::GetLegend(THStack* background, std::shared_ptr<Channel> processes, TH1* data, TH1* signal, bool includeSignal, bool includeData)
{
    //Draws the legend
    auto legend = new TLegend(0.55 - (right/width), 0.75 - (top/height), 1 - (right/width), 1 - (top/height));
    legend->SetTextSize(0.04);
    std::string name;
    TString toAdd;
    if (includeData)
    {
        name = processes->getNamesWithLabel("data").at(0); 
        toAdd = name;
        legend->AddEntry(data, " " + toAdd, "L");
    }
    else
    {
        name = processes->getNamesWithLabel("signal").at(0); 
        toAdd = name;
        legend->AddEntry(signal, " Signal", "L");
    }
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

TLegend* PlotFormatter::GetSimpleLegend(std::vector<TH1*> hists, std::vector<std::string> names)
{
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
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

TLegend* PlotFormatter::GetSimpleLegend(std::vector<TH1*> hists, std::vector<TString> names)
{
    //Draws the legend
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
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

TLegend* PlotFormatter::GetSuperImposedLegend(std::shared_ptr<Channel> processes, HistVariable histvariable)
{
    auto legend = new TLegend(0.8-(right/width), 0.85-(top/height), 1-(right/width), 1-(top/height));
    legend->SetTextSize(0.02);
    int count = 0;
    std::string name;
    TString toAdd;
    for(const auto&& obj : *(processes->getStack(histvariable, "signal", true))->GetHists()) {
        name = processes->getNamesWithLabel("signal").at(count); 
        toAdd = name;
        legend->AddEntry(obj, " " + toAdd, "L");
        count++;
    }
    count = 0;
    for(const auto&& obj2 : *(processes->getStack(histvariable, "background", true))->GetHists()) {
        name = processes->getNamesWithLabel("background").at(count);
        toAdd = name;
        legend->AddEntry(obj2, " " + toAdd, "L");
        count++;
    }
    return legend;
}

TLegend* PlotFormatter::GetStackedLegend(std::shared_ptr<Channel> processes, THStack* hists)
{
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
    return legend;
}

void PlotFormatter::DrawOtherHistograms(std::vector<TH1*>& hists, int& firstIndex)
{
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
            hist->Draw("HIST SAME");
            histVector.push_back(hist);
        }
    }
}

void PlotFormatter::GetImposedOrder(THStack*& background, THStack*& signal, THStack*& first, THStack*& second)
{
    if(signal->GetMaximum() > background->GetMaximum()) {
        first = signal;
        second = background;
    }
    else {
        first = background;
        second = signal;
    }
}

void PlotFormatter::GetOrder(std::vector<TH1*>& hists, TH1*& first, int& firstIndex, double& maximum)
{
    for(TH1* hist : hists) {
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
    }
}

void PlotFormatter::Bin(std::vector<TH1*>& hists, TH1*& first, int& firstIndex, double& maximum, int& count, bool scaleToExpected)
{
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

    for(TH1* hist : hists) 
    {
        if(scaleToExpected) 
        {
           if(hist->Integral() != 0 && !std::isnan(hist->Integral())) 
           {
	        hist->Scale(1/hist->Integral());
		    hist->SetFillColor(kWhite);
           }
	    
	    }
        if(hist->GetMaximum() > maximum) {
            maximum = hist->GetMaximum();
            first = hist;
            firstIndex = find(hists.begin(), hists.end(), hist) - hists.begin();
        }
	    count++;
    }
}

// void PlotFormatter::SetErrors(std::shared_ptr<Process> process, HistVariable histVariable, TH1 *original)
// {
//     auto errorGraph = dynamic_cast<TH1*>(original->Clone());
//     std::pair<TH1*, TH1*> errorHists = process-> getUncertainties(errorGraph);
    
//     for (int i = 0; i < errorHists.first->GetNbinsX(); i++)
//     {
//         errorHists.first->SetBinContent(i, 0);
//         errorHists.first->SetBinError(i, 0);
//     }

//     for (int i = 0; i < errorHists.second->GetNbinsX(); i++)
//     {
//         errorHists.second->SetBinContent(i, 0);
//         errorHists.second->SetBinError(i, 0);
//     }

//     // errorGraph->Draw("E2 SAME");
//     // errorGraph->SetFillColor(kBlack);
// }

TH1* PlotFormatter::CreateErrorHistogram(THStack* hists, std::vector<std::shared_ptr<Process>> processes)
{
    // auto backgroundHist = hists->GetHistogram();
    // backgroundHist->SetFillColor(kBlack);
    // backgroundHist->SetFillStyle(3018);

    TList *histList = hists->GetHists();
    TIter next(histList);  
    TH1 *hist;  
    TH1* backgroundHist;
    backgroundHist = nullptr;
    while ((hist = (TH1*)next())) 
    {
        if (!backgroundHist)
        {
            backgroundHist = dynamic_cast<TH1*>(hist->Clone());
        }
        else
        {
            backgroundHist->Add(hist);
        }
    }

    auto systematicHist = Process::combineSystematics(processes, backgroundHist);

    //loop through bin by bin in backgroundHist and set the error using setBinError
    for (int i = 0; i < backgroundHist->GetNbinsX(); i++)
    {
        // for (auto hist : systematicHist)
        // {
            double error1 = systematicHist.first->GetBinContent(i) * backgroundHist->GetBinContent(i) + backgroundHist->GetBinContent(i);
            double error2 = -systematicHist.second->GetBinContent(i) * backgroundHist->GetBinContent(i) + backgroundHist->GetBinContent(i);
            std::cout << "error1: " << error1 << ", error2: " << error2 << std::endl;
            std::cout << "bin: " << backgroundHist->GetBinContent(i) << std::endl;
            // std::cout << "sum of errors: " << error1 + error2 << std::endl;
            backgroundHist->SetBinContent(i, (error1 + error2)/2);
            double highError = systematicHist.first->GetBinContent(i) * backgroundHist->GetBinContent(i);
            backgroundHist->SetBinError(i, highError);
            std::cout << "background hist error: " << backgroundHist->GetBinError(i) << std::endl;
        // }
    }

    backgroundHist->Draw("E2 SAME");
    return backgroundHist;
}