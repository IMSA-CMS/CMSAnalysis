#include "CMSAnalysis/Histograms/interface/HistogramPrototype1DGeneral.hh"

#include "TH1.h"
#include <functional>

#include <string>

HistogramPrototype1DGeneral::HistogramPrototype1DGeneral(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(const InputModule*)>function):
    HistogramPrototype1D(iname, inBins, iminimum,imaximum), 
    vecOne(function)
{}