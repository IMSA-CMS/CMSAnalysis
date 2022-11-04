#include "CMSAnalysis/DataCollection/interface/SingleParticleHist.hh"

#include "TH1.h"
#include <functional>

#include <string>

SingleParticleHist::SingleParticleHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)>function):
    HistogramPrototype1D(iname, inBins, iminimum, imaximum),
    valueFunction(function)
{}

std::vector<double> SingleParticleHist::value() const
{
    if (particle != nullptr)
    {
        return valueFunction(particle);
    }
    return std::vector<double>{};
}