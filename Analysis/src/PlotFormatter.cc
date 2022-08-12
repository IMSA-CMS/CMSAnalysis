#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TFrame.h"
#include "TStyle.h"
#include "TPad.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"
#include "TPaveStats.h"
#include "TFitResult.h"
#include <iostream>

TCanvas* PlotFormatter::formatPlot(THStack* first, THStack* second)
{
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
    
    //Change axis titles here
    first->GetHistogram()->GetXaxis()->SetTitle("GeV");
    first->GetHistogram()->GetYaxis()->SetTitle("Events");
    
    second->Draw("HIST SAME");

    canvas->Update();
    return canvas;
}