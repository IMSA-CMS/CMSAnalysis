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

TH1* SingleProcess::getHist(bool scaleToExpected) const
{
    if(scaleToExpected) {
        TH1* hist = input->getHist();
        double yield = getExpectedYield();
        double events = hist->Integral();
        std::cout << "Process: " << crossSectionName << std::endl;
        //std::cout << "Expected yield: " << yield << std::endl;
        //std::cout << "Generated events: " << events << std::endl;
        //std::cout << "Scaling by: " << yield/events << std::endl;
        hist->Scale(yield/events);
        return hist;
    }
    else {
        return input->getHist();
    }
}

int SingleProcess::getTotalEvents() const
{
    return input->getTotalEvents();
}

double SingleProcess::getExpectedYield() const
{
    double expectedYield = estimator->getExpectedYield(this, luminosity);
    return expectedYield;
}