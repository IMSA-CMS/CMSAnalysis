#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/ShapeSystematic.hh"
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


ShapeSystematic::ShapeSystematic(std::string name, TH1* ihigh, TH1* ilow, Distribution idistribution) :
    Systematic(name),
    high(ihigh),
    low(ilow),
    distribution(idistribution)
{}


std::string ShapeSystematic::getString() const
{
    return "";
}

std::pair<TH1*, TH1*> ShapeSystematic::adjustHistogram(TH1* original) const 
{
    auto graphHigh = dynamic_cast<TH1*>(high->Clone());
    auto graphLow = dynamic_cast<TH1*>(low->Clone());
    double num = original->Integral(); 
    
    double highNum = graphHigh->Integral();
    double lowNum = graphLow->Integral();
    
    double  tolerance = 1e-8;
    if(highNum > tolerance)
    {
        graphHigh->Scale(num/highNum);
    }
    if (lowNum > tolerance)
    {
        graphLow->Scale(num/lowNum);
    }
    
    return {graphHigh, graphLow};
}
 