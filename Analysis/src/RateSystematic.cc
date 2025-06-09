#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/RateSystematic.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include "TStyle.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream> //change include paths


RateSystematic::RateSystematic(std::string name, double upRate, double downRate, Distribution idistribution) :
    Systematic(name),
    upFactor(upRate),
    downFactor(downRate),   
    distribution(idistribution)
{}


std::string RateSystematic::getString() const
{
    return std::to_string(upFactor) + " " + std::to_string(downFactor);
}

std::pair<TH1*, TH1*> RateSystematic::getUncertainties(TH1* original) const 
{
    auto graphHigh = dynamic_cast<TH1*>(original->Clone());
    auto graphLow = dynamic_cast<TH1*>(original->Clone());

    for (int i = 0; i < graphHigh->GetNbinsX(); i++)
    {

        graphHigh->SetBinContent(i, upFactor);
        graphLow->SetBinContent(i, downFactor);
        
    }
    return {graphHigh, graphLow};
}
 