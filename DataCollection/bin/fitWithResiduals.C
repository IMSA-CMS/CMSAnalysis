#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>

void fitWithResiduals() {
    const char* filename = "strippedBG_numFiles1.root";
    const char* treename = "Signal";
    const char* branchname = "sumPt";
    
    TFile *file = TFile::Open(filename);
    TTree *tree = (TTree*)file->Get(treename);

    TH1F *hist = new TH1F("hist", "Histogram with Fit and Residuals", 100, 0, 100);
    tree->Draw(Form("%s>>hist", branchname));

    TF1 *fit = new TF1("fit", "gaus", 0, 100);
    hist->Fit(fit);

    int nBins = hist->GetNbinsX();
    double *x = new double[nBins];
    double *residuals = new double[nBins];

    for (int i = 1; i <= nBins; ++i) {
        x[i-1] = hist->GetBinCenter(i);
        residuals[i-1] = hist->GetBinContent(i) - fit->Eval(x[i-1]);
    }

    TGraph *residualGraph = new TGraph(nBins, x, residuals);

    TCanvas *c1 = new TCanvas("c1", "Fit with Residuals", 800, 600);
    c1->Divide(1, 2);

    c1->cd(1);
    hist->Draw();
    fit->Draw("SAME");

    c1->cd(2);
    residualGraph->Draw("AP");

    c1->SaveAs("fit_with_residuals.png");

    delete[] x;
    delete[] residuals;
    delete c1;
    delete hist;
    delete fit;
    delete residualGraph;
    file->Close();
    delete file;
}
