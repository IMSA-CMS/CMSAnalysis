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
#include "THStack.h"

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

    //TH1* hist = analysis->getDecayHist("Same Sign Inv Mass", "ZZ Background", 900, false);
    
    //Choices for plotting are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
    std::string histvariable = "Invariant Mass";
    double massTarget = 1400;

    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    std::shared_ptr<Channel> processes = channels.at(0);
    std::vector<std::string> backgroundNames = processes->getNamesWithLabel("background");
    std::vector<std::string> signalNames = processes->getNamesWithLabel("signal");
    std::vector<std::string> dataNames = processes->getNamesWithLabel("data");
    TH1* data = analysis->getDecayHist(histvariable, dataNames.at(0), massTarget, false);
    TH1* signal = analysis->getDecayHist(histvariable, signalNames.at(0), massTarget, true);
    std::vector<TH1*> backgroundHists;
    for(std::string name : backgroundNames) {
        backgroundHists.push_back(analysis->getDecayHist(histvariable, name, massTarget, true));
    }
    THStack* background = new THStack("background", "background");
    for(TH1* backgroundHist : backgroundHists) {
        //Rebin to number of background processes
        backgroundHist->Rebin(4);
        background->Add(backgroundHist);
    }    
    background->Draw("HIST");
    //data->Draw("HIST");
}