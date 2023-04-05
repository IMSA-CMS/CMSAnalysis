#include <algorithm>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"

using std::string;

TH1* getHist(string name, TFile* file);
//void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName);
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title);    

//inputFile: input root file name
void GaussianFit(std::string inputFile)
{
    TFile* refinedOutput = TFile::Open(inputFile.c_str());
    
    string originalHist = "Gamma Resolution";
    string title = "Gamma Resolution (Gaussian Fit)";
        
    auto originalPlot = getHist(originalHist.c_str(), refinedOutput);
    auto fittedPlot = (TH1D*) originalPlot->Clone(); 

    //add gaussian fit
    fittedPlot->Fit("gaus","","", -0.05, 0.05);

    string canvasName = "Gamma Resolution (Gaussian Fit)";
    string xaxis = "Resolution";
    string yaxis = "Gamma";
    process(fittedPlot, canvasName, xaxis, yaxis, title);
} 
    

TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}

//void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName){
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title){    
    //TFile* refinedOutput = TFile::Open(rootFileName.c_str(), "UPDATE");
    TCanvas* c = new TCanvas(canvasName.c_str(), canvasName.c_str());
    hist->SetStats(kFALSE);
    hist->GetXaxis()->SetTitle(xaxis.c_str());
    hist->GetYaxis()->SetTitle(yaxis.c_str());
    hist->SetTitle(title.c_str());
    hist->Write();
    hist->Draw();
    c->Update();
    c->Write();
    c->SaveAs((canvasName + ".png").c_str());
}