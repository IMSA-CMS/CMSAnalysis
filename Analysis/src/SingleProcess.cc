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

TH1* SingleProcess::getHist(std::string histType, bool scaleToExpected) const
{
    TH1* hist = input->getHist(histType);
    if(scaleToExpected) {
        double yield = getExpectedYield(histType);
        double events = hist->Integral();
        if(events != 0) {
            hist->Scale(yield/events);
        }
    }
    for(auto correction : corrections) {
        hist = correction->correctHist(hist);
    }
    return hist;
}

TH1* SingleProcess::get2DHist(std::string histType) const
{
    return input->get2DHist(histType);
}

int SingleProcess::getTotalEvents() const
{
    return input->getTotalEvents();
}

double SingleProcess::getExpectedYield(std::string dataType) const
{
    double expectedYield = estimator->getExpectedYield(this, dataType, luminosity);
    return expectedYield;
}

bool SingleProcess::checkValidity() 
{
    bool validProcess = true;
    std::vector<HistVariable> histVariables = input->getHistVariables();
    for(HistVariable histVar : histVariables) {
        if(input->getHist(histVar.getName()) == 0) {
            //std::cout << crossSectionName << " failed on " << histVar.getName() << std::endl;
            validProcess = false;
        }
    }    
    return validProcess;
}
