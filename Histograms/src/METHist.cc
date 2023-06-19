#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"

METHist::METHist(std::shared_ptr<METModule> iMETModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
mETModule(iMETModule)
{}
std::vector<double> METHist::value() const
{
    return {mETModule->getMet()};
}

