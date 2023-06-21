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

class RateSystematic : public Systematic
{
    public:
        std::string getString() const;

    private:
        double factor;
        Distribution distribution;


};

#endif
