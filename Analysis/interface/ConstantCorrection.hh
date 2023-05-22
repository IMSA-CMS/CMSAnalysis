#ifndef CONSTANTCORRECTION_HH
#define CONSTANTCORRECTION_HH

#include "TH1.h"
#include "CMSAnalysis/Analysis/interface/Correction.hh"

class ConstantCorrection : public Correction
{
    public: 
        ConstantCorrection(double scaleFactor) : scalar(scaleFactor) {}
        TH1* correctHist(TH1* hist) const override; 

    private:
        double scalar;
};

#endif