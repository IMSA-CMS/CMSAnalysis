#include "CMSAnalysis/Analysis/interface/SimpleEstimator.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "TH1.h"

double SimpleEstimator::getExpectedYield(const SingleProcess* process, std::string dataType, double luminosity) const {
    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(process->getHist(dataType));
    if (!hist) {
        throw std::runtime_error("Hist not found");
    }

    int totalEventsInt = process->getTotalEvents();
    double totaleventsran = totalEventsInt;

    //Finds crosssection (from spreadsheet)
    double crosssection = process->getCrossSection();

    // integral/fit only calibrated after firstBin  
    // needs to match the "firstBin" value in "PlotFormatter"
    int firstBin = 50;
    // mass range
    // needs to match the "upperMasslimit" value in "PlotFormatter"
    int xAxisRange = 2000;
    int numBins = hist->GetNbinsX(); 
    int lowerLimit = firstBin * (static_cast<double>(numBins) / xAxisRange);
    double efficiency = (hist->Integral(lowerLimit, numBins)) / totaleventsran;
    double signalest = crosssection * 1000 * luminosity * efficiency;
    //hist->GetNbinsX()
     //for (int i = 1; i <= 10; i++)
     //{
        //preIntegral 1.57725e+08 (62858)
        //preMax 2.86772e+07
        //totalentries 44
        //if (process->getName() == "dy10to50")
        //{
        //    std::cout << "bin" << 5 << ": " << hist->GetBinContent(5) << std::endl;
        //}
     //}
    
     /*std::cout << "postMax " << hist->GetMaximum() << std::endl;
     std::cout << "MaxContent " << hist->GetBinContent(hist->GetMaximumBin()) << std::endl;
     std::cout << "process " << process->getName() << std::endl; 
     std::cout << "totaleventsran " << totaleventsran << std::endl;
     std::cout << "totalentries " << hist->GetEntries() << std::endl;
     std::cout << "efficiency " << efficiency << std::endl;
     std::cout << "luminosity " << luminosity << std::endl;
     std::cout << "cross section " << crosssection << std::endl;
     std::cout << "postIntegral " << hist->Integral(lowerLimit, numBins) << std::endl;
     std::cout << "yield for " << process->getName() << " is " << signalest << std::endl;*/

    return signalest;
}