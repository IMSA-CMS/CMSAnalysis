#ifndef MULTISYSTEMATIC_HH
#define MULTISYSTEMATIC_HH

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



class MultiSystematic : public Systematic
{
    public:
        MultiSystematic(std::string name);
        virtual std::string getString() const override;
        virtual std::pair<TH1*, TH1*> adjustHistogram(TH1* original) const override;
        void addSystematic(std::shared_ptr<Systematic> systematic);
    private:
       std::vector<std::shared_ptr<Systematic>> systematics; 
};

#endif
