#ifndef MLSTRIPHIST_HH
#define MLSTRIPHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include <functional>
#include <string>
#include <vector>

class MLStripHist : public HistogramPrototype1D
{
public:
    MLStripHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(LeptonJet)> func);

    void setLeptonJet(LeptonJet jet);

    virtual std::vector<double> value() const override;

protected:
    LeptonJet histJet;
    std::function<std::vector<double>(LeptonJet)> MLStripFunction;
};

// Inline function definitions

inline MLStripHist::MLStripHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(LeptonJet)> func)
    : HistogramPrototype1D(iname, iNBins, iminimum, imaximum), MLStripFunction(func) {}

inline void MLStripHist::setLeptonJet(LeptonJet jet)
{
    histJet = jet;
}

inline std::vector<double> MLStripHist::value() const
{
    return MLStripFunction(histJet);
}

#endif // MLSTRIPHIST_HH
