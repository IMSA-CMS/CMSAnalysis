#include "TH1.h"
#include "CMSAnalysis/Analysis/interface/Correction.hh"
#include "CMSAnalysis/Analysis/interface/ConstantCorrection.hh"

TH1* ConstantCorrection::correctHist(TH1* hist) const
{
    TH1* histClone = dynamic_cast<TH1*>(hist->Clone());
    histClone->Scale(scalar);
    return histClone;
}