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
        Systematic(std::string name, double value);
        virtual ~Systematic(){}
        std::string getName() const {return name;}
        virtual std::string getString() const = 0;
    private:
        const std::string name;
        const double value;
};

#endif