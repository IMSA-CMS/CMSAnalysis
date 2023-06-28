#include "CMSAnalysis/Utility/interface/CollectionHist.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

#include "TH1.h"
#include <functional>

#include <string>

CollectionHist::CollectionHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)>function):
    HistogramPrototype1D(iname, inBins, iminimum, imaximum),
    collection(std::make_shared<ParticleCollection<Particle>>()),
    valueFunction(function)
{}

void CollectionHist::clear()
{
    collection = std::make_shared<ParticleCollection<Particle>>();
}


void CollectionHist::setCollection(std::shared_ptr<ParticleCollection<Particle>> particleCollection) 
{
    collection = particleCollection;
} 

 
std::vector<double> CollectionHist::value() const
{
    if (collection)
    {
        return valueFunction(collection);
    }
    return std::vector<double>{};
}