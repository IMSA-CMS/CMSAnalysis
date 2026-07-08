#include "CMSAnalysis/Analysis/interface/TrivialEstimator.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "TH1.h"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

double TrivialEstimator::getExpectedYield(const SingleProcess* process, const HistVariable& dataType, double luminosity) const
{
    //Takes the histogram wanted from the file, assigns it hist
    //TH1 *hist = dynamic_cast<TH1 *>(process->getHist(dataType, false));
    //std::cout << "TrivialEstimator 1" << std::endl;
    // if (isData)
    // {
    //     return process->getTotalEvents();
    // }

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
    if (!hist)
    {
       return 0;
    }
    
    return hist->Integral();
}

