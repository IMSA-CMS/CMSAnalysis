#include "CMSAnalysis/Utility/interface/HistParams.hh"

#include "TH1.h"
#include <functional>
#include <string>


HistParams::HistParams(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)>function):
    valueFunction(function),
    name(iname), 
    bins(inBins),
    minimum(iminimum),
    maximum(imaximum)
    {}

std::string HistParams::getName() const {
    return name;
}

int HistParams::getBins() const {
    return bins;
}

double HistParams::getMinimum() const {
    return minimum;
}

double HistParams::getMaximum() const {
    return maximum;
}

std::function<std::vector<double>(Particle)> HistParams::getValueFunction() const {
    return valueFunction;
}