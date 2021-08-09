#include <iostream>
#include <TH1.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TAxis.h>

void InvariantMassFitScript()
{    
    //Opens my file, assigns it f
    TFile *f = new TFile("DYMM50Sorted.root");

    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1*>(f->Get("2Same Sign Invariant Mass"));
    if (!hist)
        throw std::runtime_error ("Hist not found");

    //Creates a canvas
    TCanvas *canvas = new TCanvas("canvas", 0, 1500);
    canvas->SetLogy();

    //Creates an exponential fit function
    TF1 *fitfunc = new TF1("fitfunc", "[0]*pow(x, [1])", 200, 1300);

    //Draws the hist
    hist->Draw();

    //Fits the histogram with the fit function
    hist->Fit(fitfunc, "", "", 200, 1300);

    //Finds the number of events in the 4 lepton hist
    TH1F *hist4l = (TH1F*)f->Get("4Same Sign Invariant Mass");
    double events4lhist = hist4l->GetEntries();
    cout << "\nevents4lhist \n";
    cout << events4lhist;

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = 2982000;
    cout << "\ntotaleventsran \n";
    cout << totaleventsran;

    //Finds 4 lepton fraction
    double fraction4l = events4lhist/totaleventsran;
    cout << "\nfraction4l \n";
    cout << fraction4l;

    //Finds the integral of the fit from 760-840
    double acceptedfitintegral = fitfunc->Integral(760, 840);
    cout << "\nacceptedfitintegral \n";
    cout << acceptedfitintegral;
    
    //Finds the integral of the fit from 150-1300
    double rangefitintegral = fitfunc->Integral(150, 1300);
    cout << "\nrangefitintegral \n";
    cout << rangefitintegral;

    //Calculates ratio between both fit integrals
    double fitratio = acceptedfitintegral/rangefitintegral;
    cout << "\nfitratio \n";
    cout << fitratio;

    //Finds the integral of the histogram from 0-1500
    double totalhistintegral = hist->Integral(0, 1500);
    cout << "\ntotalhistintegral \n";
    cout << totalhistintegral;

    //Finds the integral of the histogram from 150-1300
    double bin150 = hist->FindBin(150);
    double bin1300 = hist->FindBin(1300);
    double rangehistintegral = hist->Integral(bin150, bin1300);
    cout << "\nrangehistintegral \n";
    cout << rangehistintegral;

    //Finds histogram fraction
    double histfraction = fitratio*(rangehistintegral/totalhistintegral);
    cout << "\nhistfraction \n";
    cout << histfraction;

    //Finds luminosity (from spreadsheet)
    double luminosity = 137;
    cout << "\nluminosity \n";
    cout << luminosity;

    //Finds crosssection (from spreadsheet)
    double crosssection = 2113;
    cout << "\ncrosssection \n";
    cout << crosssection;

    //Finds signal
    double signal = fraction4l*crosssection*1000*luminosity*histfraction;
    cout << "\nsignal \n";
    cout << signal;
}