#ifndef SYSTEMATIC_HH
#define SYSTEMATIC_HH

#include "SingleProcess.hh"
#include "Process.hh"
#include "HistVariable.hh"
#include "TH2F.h"
#include <string>
#include <vector>

class Systematic
{
    public:
        Systematic(std::string name);
        virtual ~Systematic(){}
        std::string getName() const {return name;}
        virtual std::string getString() const = 0;

        static std::shared_ptr <Systematic> makeSystematicFromHist(TH1*original,TH1*up,TH1*down);

        // should return deltax/x in each bin
        // first histogram is the high uncertainty, second is the low uncertainty
        virtual std::pair<TH1*, TH1*> getUncertainties(TH1* original) const = 0;
    private:
        const std::string name;
};

#endif