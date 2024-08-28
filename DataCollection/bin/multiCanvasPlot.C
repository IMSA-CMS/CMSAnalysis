#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

void multiCanvasPlot() {
    
    const char* treename = "Signal";
    const char* branchname = "sumPt";

    TFile *file1 = TFile::Open("strippedBG_numFiles1.root");
    TFile *file2 = TFile::Open("BDTFiles/strippedBG_LMSS_numFiles1.root");
    TFile *file3 = TFile::Open("BDTFiles/strippedBG_HMSS_numFiles1.root");
    TFile *file4 = TFile::Open("BDTFiles/strippedBG_HMOS_numFiles1.root");

    TTree *tree1 = (TTree*)file1->Get(treename);
    TTree *tree2 = (TTree*)file2->Get(treename);
    TTree *tree3 = (TTree*)file3->Get(treename);
    TTree *tree4 = (TTree*)file4->Get(treename);

    TH1F *hist1 = new TH1F("All", "All events", 100, 0, 100);
    TH1F *hist2 = new TH1F("LMSS", "Low mass same sign", 100, 0, 100);
    TH1F *hist3 = new TH1F("HMSS", "High mass same sign", 100, 0, 100);
    TH1F *hist4 = new TH1F("HMOS", "High mass opposite sign", 100, 0, 100);

    tree1->Draw(Form("%s>>All", branchname));
    tree2->Draw(Form("%s>>LMSS", branchname));
    tree3->Draw(Form("%s>>HMSS", branchname));
    tree4->Draw(Form("%s>>HMOS", branchname));

    TCanvas *c1 = new TCanvas("multiplot", "Background sumPt Graphs", 1200, 800);
    c1->Divide(2, 2);

    c1->cd(1);
    hist1->Draw();
    c1->cd(2);
    hist2->Draw();
    c1->cd(3);
    hist3->Draw();
    c1->cd(4);
    hist4->Draw();

    c1->SaveAs("multi_canvas_plot.png");

    delete c1;
    delete hist1;
    delete hist2;
    delete hist3;
    delete hist4;

    file1->Close();
    file2->Close();
    file3->Close();
    file4->Close();

    delete file1;
    delete file2;
    delete file3;
    delete file4;
}