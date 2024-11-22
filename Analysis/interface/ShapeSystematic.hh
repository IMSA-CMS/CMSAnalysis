#ifndef SHAPESYSTEMATIC_HH
#define SHAPESYSTEMATIC_HH

#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Systematic.hh"
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
#include <fstream>



class ShapeSystematic : public Systematic
{
    public:
        enum class Distribution
        {
            LogNormal
        };
        ShapeSystematic(std::string name, TH1* high, TH1* low, Distribution distribution = Distribution::LogNormal);
        virtual std::string getString() const override;
        virtual std::pair<TH1*, TH1*> adjustHistogram(TH1* original) const override;
    private:
        TH1* high;
        TH1* low;
        Distribution distribution;
};

#endif
