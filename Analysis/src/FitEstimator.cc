#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include <iostream>
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFitResult.h"
#include <sstream>
#include <fstream>
#include <array>
#include <iostream>
#include <string>
#include <cstdlib>
#include "TH2.h"
#include "TH2F.h"
#include "TFile.h"

double FitEstimator::getExpectedYield(double luminosity) const 
{
//Opens my file, assigns it f
    TFile *f = new TFile(getProcess()->getName().c_str()); //Analysis filename //Change this for different files

    TFile *fitfile = new TFile(getProcess()->getFitName().c_str());

    //Find total events from file
    //TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    //std::string totalEventsStr = getProcess()->getEventsExpected()->GetString().Data();
    int totalEventsInt = getProcess()->getTotalEvents();

    //Takes the fit histogram wanted from the file, assigns it hist
    TH2 *hist2D = dynamic_cast<TH2 *>(getProcess()->getFitHist()); //Change this for different fit

    TH1 *hist = hist2D->ProjectionX("_px", 0, -1, "E");

    //Creates a power law fit function
    // TF1 *fitfunc = new TF1("fitfunc", "[0]*pow((x + [2]), [1])", 150, 3000);
    TF1 *fitfunc = new TF1("fitfunc", "[0] * pow(x, [1])", 150, 3000);

    //Fits the histogram with the fit function
    // Root's fitting is lousy, so we have to do it again and again
    // One would think a minimization algorithm could do this itself?
    double func = std::numeric_limits<double>::max();
    const double tolerance = .1;
    while (true)
    { //Removed Q
        auto result = hist->Fit(fitfunc, "RBSLQ0", "", 150, 3000);
        double newVal = result->MinFcnValue();
        if (func - newVal > tolerance)
        {
            func = newVal;
        }
        else
        {
            break;
        }
    }

    //Pulls analysis hist from file
    //TH2 *histanalysis2D = (TH2 *)f->Get(getProcess()->getHist()); //Change this for different hist analysis

    TH2* histanalysis2D = dynamic_cast<TH2 *>(getProcess()->getHist());
    
    TH1 *histanalysis = histanalysis2D->ProjectionX("_px", 0, -1, "E");
    
    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers

    //Takes the integral of the analysis hist
    double eventsanalysishist = histanalysis->GetEntries();

    //Defines out mass range and takes the integral of the fit for that range



    double acceptedCenter = massTarget(); //POINTER!!! WHICH ONE?????? DONT MISS



    double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
    double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
    double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);

    //Finds the integral of the fit from 150-1300
    double rangefitintegral = fitfunc->Integral(150, 2000);
    
    //Finds the integral of the analysis histogram from 0-1500
    double totalanalysishistintegral = histanalysis->Integral();

    //Finds the # of 4L events in the fit range divided by total events in the analysis hist
    double analysis150bin = histanalysis->FindBin(150);
    double analysis1700bin = histanalysis->FindBin(2000);
    double eventanalysisfitrange = exp(log(histanalysis->Integral(analysis150bin, analysis1700bin)) - log(totalanalysishistintegral));
    
    //Calculates ratio between both fit integrals
    double fitratio = exp(log(acceptedfitintegral) - log(rangefitintegral));

    //Finds histogram fraction
    double histfraction = exp(log(fitratio) + log(eventanalysisfitrange));

    //Finds efficiency
    double efficiency = exp(log(eventsanalysishist) - log(totaleventsran));

    //Finds crosssection (from spreadsheet)
    double crosssection = getProcess()->getCrossSection(); //Change this for different cross section

    //Finds background
    double backgroundest = exp(log(efficiency) + log(1000) + log(crosssection) + log(luminosity) + (2 * log(histfraction)));
    return backgroundest;
    //For efficiency lose: log(1000) + log(crosssection) + log(luminosity)

}
