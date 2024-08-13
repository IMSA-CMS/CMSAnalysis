#ifndef MLSTRIPHIST_HH
#define MLSTRIPHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include <functional>
#include <string>

class MLStripHist : public HistogramPrototype1D
{
public:
    MLStripHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>()> func)
        : HistogramPrototype1D(iname, iNBins, iminimum, imaximum), MLStripFunction(func) {}
    
    std::vector<double> value() const override { return MLStripFunction(); }

private:
    std::function<std::vector<double>()> MLStripFunction;
};

#endif // MLSTRIPHIST_HH