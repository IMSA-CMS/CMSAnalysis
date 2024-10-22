#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TF1.h>

void fitGaussian() {
    const char* filename = "strippedBG_numFiles1.root";
    const char* treename = "Signal";
    const char* branchname = "sumPt";

    TFile *file = TFile::Open(filename);
    TTree *tree = (TTree*)file->Get(treename);

    TH1F *hist = new TH1F("hist", "Gaussian Fit", 100, 0, 100);
    tree->Draw(Form("%s>>hist", branchname));

    TF1 *gaus = new TF1("gaus", "gaus", 0, 100);
    hist->Fit(gaus);

    TCanvas *c1 = new TCanvas("fit", "Gaussian Fit", 800, 600);
    hist->Draw();
    gaus->Draw("SAME");
    c1->SaveAs("gaussian_fit.png");

    delete c1;
    delete hist;
    delete gaus;
    file->Close();
    delete file;
}
