#ifndef CONSTANTCORRECTION_HH
#define CONSTANTCORRECTION_HH

#include "TH1.h"

class ConstantCorrection 
{
    public: 
        ConstantCorrection(double scaleFactor) : scalar(scaleFactor) {}
        TH1* correctHist(TH1* hist) const override {return hist->Scale(scalar);} 

    private:
        double scalar;
};

#endif