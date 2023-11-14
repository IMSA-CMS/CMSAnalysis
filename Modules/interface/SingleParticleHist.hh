#ifndef SINGLEPARTICLEHIST_HH
#define SINGLEPARTICLEHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/HistParams.hh"

#include <functional>

#include <string>
#include <vector>

class HistParams;

//Histogram is assigned a particle rather than an inputmodule
//Particle must be updated for each event and cleared if there is none
//This class works in conjuction with ParticleType

class SingleParticleHist : public HistogramPrototype1D
{
  public:
    SingleParticleHist(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)>function);
    SingleParticleHist(const HistParams& params);
    //Clones histogram / This is necessary for to move them from ParticleType
   // SingleParticleHist clone() {return SingleParticleHist(getName(),getNBins(),getMinimum(),getMaximum(),valueFunction);};

    //Sets nullparticle to hist to avoid null pointer error
    void clear() {setParticle(Particle::nullParticle());};

    //Sets the Particle
    void setParticle(Particle part) {particle = part;}; 

    //This function is necessary to change name from default in ParticleType / called in EventModule
    //void changeName (std::string newName) {setName(newName);};

    // value function which is called in HistMod
    virtual std::vector<double> value() const override;

  protected:
    Particle particle{Particle::nullParticle()};
    std::function<std::vector<double>(Particle)> valueFunction;
};

#endif
