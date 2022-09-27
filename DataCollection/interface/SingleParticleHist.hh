#ifndef SINGLEPARTICLEHIST_HH
#define SINGLEPARTICLEHIST_HH

#include "HistogramPrototype1D.hh"
#include "Particle.hh"

#include <functional>

#include <string>
#include <vector>

//Histogram is assigned a particle rather than an inputmodule
//Particle must be updated for each event
class SingleParticleHist : public HistogramPrototype1D
{
  public:
    SingleParticleHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)>function);
    
    SingleParticleHist clone() {return SingleParticleHist(getName(),getNBins(),getMinimum(),getMaximum(),valueFunction);};
    void clear() {setParticle(nullptr);};

    void setParticle(Particle part) {particle = part;}; 
    void changeName (std::string newName) {setName(newName);};

    virtual std::vector<double> value() const override;

  protected:
    Particle particle;
    std::function<std::vector<double>(Particle)> valueFunction;
};

#endif
