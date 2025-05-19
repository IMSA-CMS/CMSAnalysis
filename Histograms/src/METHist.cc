#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"

METHist::METHist(const std::string& iname, int iNBins, double iminimum, double imaximum):
HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
{}
std::vector<double> METHist::value() const
{
    //std::cout << "metModule TEST: " << getInput()->getMET() << std::endl;
    return {getInput()->getMET()};
}

