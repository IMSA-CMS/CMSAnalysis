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
        //std::cout << "Process: " << crossSectionName << std::endl;
        TH1* hist = input->getHist();
        double yield = getExpectedYield();
        double events = hist->Integral();
        hist->Scale(yield/events);
        return hist;
    }
    else {
        return input->getHist();
    }
}

TH1* SingleProcess::get2DHist() const
{
    return input->get2DHist();
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