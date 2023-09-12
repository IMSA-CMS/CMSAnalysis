#include "CMSAnalysis/Utility/interface/CollectionHistParams.hh"

#include "TH1.h"
#include <functional>
#include <string>


CollectionHistParams::CollectionHistParams(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)>function):
    name(iname),
    NBins(iNBins),
    minimum(iminimum),
    maximum(imaximum),
    valueFunction(function)
{}

void CollectionHistParams::setName(std::string newName)
{
    name = newName;
}

std::string CollectionHistParams::getName() const {
    return name;
}

int CollectionHistParams::getBins() const {
    return NBins;
}

double CollectionHistParams::getMinimum() const {
    return minimum;
}

double CollectionHistParams::getMaximum() const {
    return maximum;
}

std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)> CollectionHistParams::getValueFunction() const {
    return valueFunction;
}
