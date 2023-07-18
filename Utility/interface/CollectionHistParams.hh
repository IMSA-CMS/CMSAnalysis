#ifndef CollectionHistParams_HH
#define CollectionHistParams_HH

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Particle; // Forward declaration of Particle class

// Define ParticleCollection class if necessary
template<typename T>
class ParticleCollection;

class CollectionHistParams {
private:
    std::string name;
    int NBins;
    double minimum;
    double maximum;
    std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)> valueFunction;

public:
    CollectionHistParams(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)> function);

    std::string getName() const;
    int getNBins() const;
    double getMinimum() const;
    double getMaximum() const;
    std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)> getValueFunction() const;
};

#endif // CollectionHistParams_HH
