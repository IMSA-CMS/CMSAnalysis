#include "CMSAnalysis/Modules/interface/SingleParticleHist.hh"
#include "CMSAnalysis/Utility/interface/HistParams.hh"
#include "TH1.h"
#include <functional>

#include <string>

SingleParticleHist::SingleParticleHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)>function):
    HistogramPrototype1D(iname, inBins, iminimum, imaximum),
    valueFunction(function)
{}
SingleParticleHist::SingleParticleHist(const HistParams& params):
    SingleParticleHist(params.getName(), params.getBins(), params.getMinimum(), params.getMaximum(), params.getValueFunction() )
{}
std::vector<double> SingleParticleHist::value() const
{
    if (particle != Particle::nullParticle())
    {
        return valueFunction(particle);
    }
    return std::vector<double>{};
}