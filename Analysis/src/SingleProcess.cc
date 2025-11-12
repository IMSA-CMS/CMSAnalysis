#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/Input.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "TFile.h"
#include "TH1.h"
#include <string>
#include <vector>
#include <iostream>
#include "TH2.h"
#include "TH2F.h"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Correction.hh"
#include <unordered_map>

TH1* SingleProcess::getHist(HistVariable histType, bool scaleToExpected) const
{
    //std::cout << "SingleProcess 1" << std::endl;
    //std::cout << getName() << std::endl;
    TH1* hist = input->getHist(histType);
    if (!hist) {
       std::cout << "Hist not found in SingleProcess: " << getName() << std::endl;
       return nullptr;
    }
    if(scaleToExpected) 
    {
        double yield = getExpectedYield(histType);
        double events = hist->Integral();
        if(events > 0) 
        {
            hist->Scale(yield/events);
            // std::cout << "Yield: " << yield << " Events: " << events << "\n";
            // std::cout << getName() << " Scale: " << (yield/events) << "\n";
        }
    }
    //std::cout << "SingleProcess 2" << std::endl;

    //std::cout << "SingleProcess 3" << std::endl;
    return hist;
}

TH1* SingleProcess::get2DHist(HistVariable histType) const
{
    return input->get2DHist(histType);
}

int SingleProcess::getTotalEvents() const
{
    return input->getTotalEvents();
}

double SingleProcess::getExpectedYield(HistVariable dataType) const
{
    double expectedYield = estimator->getExpectedYield(this, dataType, luminosity);
    return expectedYield;
}
