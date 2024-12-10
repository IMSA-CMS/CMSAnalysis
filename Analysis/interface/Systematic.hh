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
        virtual std::pair<TH1*, TH1*> adjustHistogram(TH1* original) const = 0;
    private:
        const std::string name;
};

#endif