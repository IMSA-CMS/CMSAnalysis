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

std::pair<TH1*, TH1*> RateSystematic::getUncertainties(TH1* original) const 
{
    std::cout << "Original graph: " << original->Integral() << std::endl;
    auto graphHigh = dynamic_cast<TH1*>(original->Clone());
    auto graphLow = dynamic_cast<TH1*>(original->Clone());

    for (int i = 0; i < graphHigh->GetNbinsX(); i++)
    {

        graphHigh->SetBinContent(i, factor);
        graphLow->SetBinContent(i, factor);
        
    }

    // double scaleUp = factor;
    // graphHigh->Scale(scaleUp);
    // // std::cout << "Scale up: " << graphHigh->Integral() << std::endl;
    // std::cout << "High scale factor: " << scaleUp << std::endl;
    // double scaleDown = factor;
    // graphLow->Scale(scaleDown);
    // // std::cout << "Scale down: " << graphLow->Integral() << std::endl;
    // std::cout << "Low scale factor: " << scaleDown << std::endl;
    return {graphHigh, graphLow};
}
 