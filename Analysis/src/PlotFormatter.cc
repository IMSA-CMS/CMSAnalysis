#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TString.h"
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
#include "TPaveStats.h"
#include "TFitResult.h"
#include <iostream>
#include <memory>
#include <vector>
 
TCanvas* PlotFormatter::superImposedStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle)
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
    int width = 800;
    int height = 600;
 
    float top = 0.08*height;
    float bottom = 0.12*height;
    float left = 0.12*width;
    float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
   
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
 
    writeText(width, height, top, bottom, left, right, false, extraText);
   
    //Draws the other histogram    
    second->Draw("HIST SAME");

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::superImposedHist(std::shared_ptr<Channel> processes, HistVariable histvariable, bool scaleToExpected, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle) {
    std::vector<TH1*> hists;
    for(std::shared_ptr<Process> process : processes->getProcesses()) {
        hists.push_back(process->getHist(histvariable, scaleToExpected));
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
    int width = 800;
    int height = 600;
 
    float top = 0.08*height;
    float bottom = 0.12*height;
    float left = 0.12*width;
    float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    gStyle->SetOptStat(0);

    //Draws the histogram with more events first (bigger axis)
    first->Draw("HIST");
 
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
 
    writeText(width, height, top, bottom, left, right, false, extraText);
   
    //Draws the other histogram    
    for(TH1* hist : hists) {
        if(find(hists.begin(), hists.end(), hist) - hists.begin() != firstIndex) {
            hist->Draw("HIST SAME");
        }
    }

    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simple1DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool scaleToExpected, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle) {
    TH1* hist = process->getHist(histvariable, scaleToExpected);
 
    //Setting size and margins
    int width = 800;
    int height = 600;
 
    float top = 0.08*height;
    float bottom = 0.12*height;
    float left = 0.12*width;
    float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram
    hist->Draw("HIST");
   
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);

    writeText(width, height, top, bottom, left, right, false, extraText);
   
    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simple2DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle) {
    TH2* hist = process->get2DHist(histvariable);
 
    //Setting size and margins
    int width = 800;
    int height = 600;
 
    float top = 0.08*height;
    float bottom = 0.12*height;
    float left = 0.12*width;
    float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram
    hist->Draw("COLZ");
   
 
    //Change axis and graph titles here
    hist->GetXaxis()->SetTitle(xAxisTitle);
    hist->GetYaxis()->SetTitle(yAxisTitle);

    writeText(width, height, top, bottom, left, right, false, extraText);
   
    canvas->Update();
    return canvas;
}

TCanvas* PlotFormatter::simpleStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle) {
    THStack* hists = processes->getStack(histvariable, "", true);
 
    //Setting size and margins
    int width = 800;
    int height = 600;
 
    float top = 0.08*height;
    float bottom = 0.12*height;
    float left = 0.12*width;
    float right = 0.04*width;

    TCanvas* canvas = makeFormat(width, height, top, bottom, left, right);

    //Draws the histogram with more events first (bigger axis)
    hists->Draw("HIST");
   
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
 
    writeText(width, height, top, bottom, left, right, false, extraText);

    canvas->Update();
    return canvas;
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

void PlotFormatter::writeText(int w, int h, float t, float b, float l, float r, bool drawLogo, TString extraText) {
    //Writes CMS logo and integrated luminosity
    int align_ = 13;
    TString lumiText = "14 TeV";
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