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


RateSystematic::RateSystematic(std::string name, double rate, Distribution idistribution) :
    Systematic(name),
    factor(rate),
    distribution(idistribution)
{}


std::string RateSystematic::getString() const
{
    return std::to_string(factor);
}

std::pair<TH1*, TH1*> RateSystematic::adjustHistogram(TH1* original) const 
{
    double originalContent = original->GetBinContent(15);
    std::cout<<"original content: " << originalContent << std::endl;
    auto graphHigh = dynamic_cast<TH1*>(original->Clone());
    auto graphLow = dynamic_cast<TH1*>(original->Clone());

    double scaleUp = 1 + factor;
    graphHigh->Scale(scaleUp);
    double highContent = graphHigh->GetBinContent(15);
    std::cout<<"high graph content: " << highContent << std::endl;

    double scaleDown = 1 - factor;
    graphLow->Scale(scaleDown);
    double lowContent = graphLow->GetBinContent(15);
    std::cout<<"low graph content: " << lowContent << std::endl;

    return {graphHigh, graphLow};
}
 