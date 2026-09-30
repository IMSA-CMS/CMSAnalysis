#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/ShapeSystematic.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream> //change include paths


ShapeSystematic::ShapeSystematic(std::string name, Distribution idistribution) :
    Systematic(name),
    distribution(idistribution)
    
{}


std::string ShapeSystematic::getString() const
{
    return "";
}

std::pair<TH1*, TH1*> ShapeSystematic::getUncertainties(TH1* original, HistVariable histVar, const Process* process) const 
{
    histVar.setSystematic(ScaleFactor::SystematicType::Up, getName());
    TH1* graphHigh = process->getHist(histVar, true);
    if (graphHigh->GetNbinsX() == original->GetNbinsX())
    {
        graphHigh->Divide(original);
    }

    histVar.setSystematic(ScaleFactor::SystematicType::Down, getName());
    TH1* graphLow = process->getHist(histVar, true);
        if (graphLow->GetNbinsX() == original->GetNbinsX())
    {
        graphLow->Divide(original);
    }
    
    histVar.setSystematic(ScaleFactor::SystematicType::Nominal, getName());
    // double num = original->Integral(); 
    
    // double highNum = graphHigh->Integral();
    // double lowNum = graphLow->Integral();
    
    // double  tolerance = 1e-8;
    // if(highNum > tolerance)
    // {
    //     graphHigh->Scale(num/highNum);
    // }
    // if (lowNum > tolerance)
    // {
    //     graphLow->Scale(num/lowNum);
    // }
    
    return {graphHigh, graphLow};
}
 