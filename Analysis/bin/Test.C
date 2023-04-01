#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsChannelsAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include <vector>
#include <memory>
#include <iostream>
#include "TH1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"

void Test() {
    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    for(std::shared_ptr<Channel> leptonBackgrounds : analysis->getChannels()) {
        for(std::string processName : leptonBackgrounds->getNames()) {
            //Change this line to make the described name your signal process name.
            if(processName == "Higgs Signal") {
                leptonBackgrounds->labelProcess("signal", processName);
            }
            else if(processName == "Higgs Data") { //This line is only used for complete plots
                leptonBackgrounds->labelProcess("data", processName);
            }
            else {
                leptonBackgrounds->labelProcess("background", processName);
            }
        }
    }
    int w = 800;
    int h = 600;
    float t = 0.08*h;
    float b = 0.12*h;
    float l = 0.12*w;
    float r = 0.04*w;
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

    TH1* hist = analysis->getHiggsHist("Same Sign Inv Mass", 900);
    hist->Draw("HIST");
}