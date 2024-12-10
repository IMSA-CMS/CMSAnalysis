#ifndef RATESYSTEMATIC_HH
#define RATESYSTEMATIC_HH

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



class RateSystematic : public Systematic
{
    public:
        enum class Distribution
        {
            LogNormal
        };
        enum class Limits
        {
            Percentages,
            Factors,
            Absolute
        };
        RateSystematic(std::string name, double rate, Distribution distribution = Distribution::LogNormal);
        virtual std::string getString() const override;
        virtual std::pair<TH1*, TH1*> adjustHistogram(TH1* original) const override;
    private:
        double factor;
        Distribution distribution;


};

#endif
