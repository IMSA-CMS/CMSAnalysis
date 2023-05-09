#ifndef CORRECTION_HH
#define CORRECTION_HH

#include "TH1.h"

class Correction 
{
    public:
        virtual ~Correction() {}
        virtual TH1* correctHist(TH1* hist) const = 0;
};

#endif