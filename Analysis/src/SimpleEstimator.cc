#include "CMSAnalysis/Analysis/interface/SimpleEstimator.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "TH1.h"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

double SimpleEstimator::getExpectedYield(const SingleProcess* process, HistVariable dataType, double luminosity) const {
    //Takes the histogram wanted from the file, assigns it hist
    //TH1 *hist = dynamic_cast<TH1 *>(process->getHist(dataType, false));
    //std::cout << "SimpleEstimator 1" << std::endl;
    TH1 *hist;
    try
    {
        hist = dynamic_cast<TH1 *>(process->getHist(dataType, false));
    }
    catch (std::runtime_error& error)
    {
        std::cout << "Error: " << error.what();
        return 0;
    }   
    if (!hist) {
       return 0;
    }
    //std::cout << "SimpleEstimator 2" << std::endl;
    int totalEventsInt = process->getTotalEvents();
    double totaleventsran = totalEventsInt;
    //Finds crosssection (from spreadsheet)
    double crosssection = process->getCrossSection();

//without integral code
/*
    double efficiency = hist->GetEntries() / totaleventsran;
    double signalest = crosssection * 1000 * luminosity * efficiency;
*/
    //std::cout << "SimpleEstimator 3" << std::endl;
 
//commented out integral code
// /*
    // integral/fit only calibrated after firstBin  
    // needs to match the "firstBin" value in "PlotFormatter" if using integral scaling
    int firstBin = 50;
    // mass range
    // needs to match the "upperMasslimit" value in "PlotFormatter" if using integral scaling
    double xAxisRange = 2000;
    //std::cout << "SimpleEstimator 3.1" << std::endl;
    int numBins = hist->GetNbinsX(); 
    //std::cout << "SimpleEstimator 3.2" << std::endl; 
    //std::cout << "numBins: " << numBins << "\n";
    int lowerLimit = firstBin * (static_cast<double>(numBins) / xAxisRange);
    double efficiency = (hist->Integral()) / totaleventsran;
    // std::cout << "SimpleEstimator 3.3" << std::endl;
    double signalest = crosssection * 1000 * luminosity * efficiency;
    // std::cout << "Passed Events: " << hist->GetEntries() << " " << hist->Integral() <<"\n";
    // std::cout << "Total Events: " << totaleventsran << "\n";
    // std::cout << "Efficiency of " << process->getName() << " is " << efficiency << std::endl;
// */

    //  std::cout << "postMax " << hist->GetMaximum() << std::endl;
    //  std::cout << "MaxContent " << hist->GetBinContent(hist->GetMaximumBin()) << std::endl;
    //  std::cout << "process " << process->getName() << std::endl; 
    //  std::cout << "totaleventsran " << totaleventsran << std::endl;
    //  std::cout << "totalentries " << hist->GetEntries() << std::endl;
    //  std::cout << "efficiency " << efficiency << std::endl;
    //  std::cout << "luminosity " << luminosity << std::endl;
    //  std::cout << "cross section " << crosssection << std::endl;
    //  std::cout << "postIntegral " << hist->Integral(lowerLimit, numBins) << std::endl;
    //  std::cout << "yield for " << process->getName() << " is " << signalest << std::endl;

    return signalest;
}