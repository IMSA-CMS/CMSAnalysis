#include <iostream>
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TH2F.h"
#include "TH2.h"
#include <sstream>
#include <fstream>
#include <array>
#include <iostream>
#include <string>
#include <cstdlib>
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"


double WindowEstimator::getExpectedYield(const SingleProcess* process, double luminosity) const
{
    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(process->getHist());
    if (!hist)
        throw std::runtime_error("Hist not found");

    int totalEventsInt = process->getTotalEvents();

    //Opens hist to analyze
    TH2 *histanalysis = (TH2F *)process->get2DHist();

    //Finds the number of events in the analyzed hist
    double eventsanalysishist = histanalysis->GetEntries();

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt;

    double efficiency = eventsanalysishist / totaleventsran;

    //Finds the integral of the analysis histogram from 0-1500
    double totalanalysishistintegral = histanalysis->Integral();
    
    double masslowaccepted = lowBound;
    double masshighaccepted = highBound;

    //Finds crosssection (from spreadsheet)
    double crosssection = process->getCrossSection();
 
    double analysisXlowboundbin = histanalysis->GetXaxis()->FindBin(masslowaccepted);
    double analysisXupboundbin = histanalysis->GetXaxis()->FindBin(masshighaccepted);
    double analysisYlowboundbin = histanalysis->GetYaxis()->FindBin(masslowaccepted);
    double analysisYupboundbin = histanalysis->GetYaxis()->FindBin(masshighaccepted);
    
    //Finds the fraction of events in the window
    double fraction4l = (histanalysis->Integral(analysisXlowboundbin, analysisXupboundbin, analysisYlowboundbin, analysisYupboundbin) / totalanalysishistintegral);

    //Finds signalest
    double signalest = efficiency * crosssection * 1000 * luminosity * fraction4l;
    
    //double signalest4 = signalest * 4;
    //return signalest4;

    return signalest;
}

double WindowEstimator::getMassTarget() const 
{
    return (lowBound + highBound) / 2;
}