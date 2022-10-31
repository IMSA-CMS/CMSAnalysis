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
#include "CMSAnalysis/Analysis/interface/HistogramFinder.hh"
#include <unordered_map>

TH1* SingleProcess::getHist(HistVariable histType, bool scaleToExpected) const
{
    if(scaleToExpected) {
        TH1* hist = input->getHist(histType);
        double yield = getExpectedYield(histType);
        double events = hist->Integral();
        hist->Scale(yield/events);
        return hist;
    }
    else {
        return input->getHist(histType);
    }
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

bool SingleProcess::checkValidity() 
{
    bool validProcess = true;
    std::shared_ptr<HistogramFinder> histFinder = input->getHistFinder();
    std::unordered_map<HistVariable, std::string> nameMap = histFinder->getMap();
    for (auto item : nameMap) {
        if(getHist(item.first) == 0) {
            validProcess = false;
        }
    }
    return validProcess;
}