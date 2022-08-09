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
#include <memory>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

TH1* FitEstimator::getFitHist(HistVariable histType) const {
    return fitInput->getHist(histType);
}

double FitEstimator::getMassTarget() const {
    return massTarget;
}

double FitEstimator::getExpectedYield(const SingleProcess* process, HistVariable dataType, double luminosity) const 
{
    int totalEventsInt = process->getTotalEvents();
    //Takes the fit histogram wanted from the file, assigns it hist 
    TH1 *hist = getFitHist(dataType);

    //Creates a power law fit function
    // TF1 *fitfunc = new TF1("fitfunc", "[0]*pow((x + [2]), [1])", 150, 3000);
    TF1 *fitfunc = new TF1("fitfunc", "[0] * pow(x, [1])", 150, 3000);

    //Fits the histogram with the fit function
    // Root's fitting is lousy, so we have to do it again and again
    // One would think a minimization algorithm could do this itself?
    double func = std::numeric_limits<double>::max();
    const double tolerance = .1;
    while (true)
    {
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
    TH1 *histanalysis = process->getHist(dataType);

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt;
    //Takes the integral of the analysis hist
    double eventsanalysishist = histanalysis->GetEntries();
    //Defines out mass range and takes the integral of the fit for that range
    double acceptedCenter = massTarget;
    double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
    double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
    double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);
    //Finds the integral of the fit from 150-2000
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
    double crosssection = process->getCrossSection();
    //Finds background
    double backgroundest = exp(log(efficiency) + log(1000) + log(crosssection) + log(luminosity) + (2 * log(histfraction)));
    if(rangefitintegral != 0) {
        return backgroundest;
    }
    else {
        return 0;
    }
}
