// #include "CMSAnalysis/DataCollection/interface/CollectionHist.hh"

// #include "TH1.h"
// #include <functional>

// #include <string>

// CollectionHist::CollectionHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(const ParticleCollection<Particle>&)>function):
//     HistogramPrototype1D(iname, inBins, iminimum, imaximum),
//     valueFunction(function)
// {}

// std::vector<double> CollectionHist::value() const
// {
//     if (collection.getNumParticles() != 0)
//     {
//         return valueFunction(collection);
//     }
//     return std::vector<double>{};
// }