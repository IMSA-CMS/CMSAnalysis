#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
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
#include "TASImage.h"
#include "TPaveStats.h"
#include "TFitResult.h"
#include <iostream>
#include <memory>
 
TCanvas* PlotFormatter::formatPlot(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle)
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
 
    TString canvasName = "Canvas";
    canvasName += width;
    canvasName += "_";
    canvasName += height;
 
    gStyle->SetOptTitle(0);

    //Creates and formats canvas
    TCanvas* canvas = new TCanvas(canvasName, canvasName, 50, 50, width, height);
    canvas->SetFillColor(0);
    canvas->SetBorderMode(0);
    canvas->SetFrameFillStyle(0);
    canvas->SetFrameBorderMode(0);
    canvas->SetLeftMargin(left/width);
    canvas->SetRightMargin(right/width);
    canvas->SetTopMargin(top/height);
    canvas->SetBottomMargin(bottom/height);
    canvas->SetTickx(0);
    canvas->SetTicky(0);
 
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
    latex.DrawLatex(1-(right/width), 1-(top/height) + 0.025, lumiText);
 
    float posX_ = (left/width) + 0.05;
    float posY_ = 0.95 - (top/height);
 
    if(drawLogo) {
        float xl_0 = posX_;
        float yl_0 = posY_ - 0.15;
        float xl_1 = posX_ + 0.15*height/width;
        float yl_1 = posY_;
        TASImage* CMS_logo = new TASImage("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/cmslogo.png");
        TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
        pad_logo->Draw();
        pad_logo->cd();
        CMS_logo->Draw("X");
        pad_logo->Modified();
        canvas->cd();
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
   
    //Draws the other histogram    
    second->Draw("HIST SAME");

    canvas->Update();
    return canvas;
}

