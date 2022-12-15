#ifndef COLLECTIONHIST_HH
#define COLLECTIONHIST_HH

#include "HistogramPrototype1D.hh"
#include "ParticleCollection.hh"
#include "Particle.hh"

class MatchingModule;

//General Histogram Type which has a ParticleCollection connected to it
//cdollection must be cleared/updated each event
class CollectionHist : public HistogramPrototype1D
{
  public:
  // Constructor / takes a value function
  CollectionHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(const ParticleCollection<Particle>&)>function);

  // Sets nullparticle to hist to avoid null pointer error
  void clear() {collection = ParticleCollection();}

  // Sets the collection
  void setCollection(const ParticleCollection<Particle>& particleCollection) {collection = particleCollection;}; 

  // value function which is called in HistMod
  virtual std::vector<double> value() const override;

  private:
  ParticleCollection<Particle> collection;

  std::function<std::vector<double>(const ParticleCollection<Particle>&)> valueFunction;
};

#endif
