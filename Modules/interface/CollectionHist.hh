#ifndef COLLECTIONHIST_HH
#define COLLECTIONHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/CollectionHistParams.hh"

#include <functional>

#include <string>
#include <vector>

template<class T>
class ParticleCollection;

//General Histogram Type which has a ParticleCollection connected to it
//collection must be cleared/updated each event
//This class works similarly to SingleParticleHist
//Implemented in ParticleType
class CollectionHist : public HistogramPrototype1D
{
  public:
  // Constructor / takes a value function
  CollectionHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)>function);
  CollectionHist(const CollectionHistParams& params);

  // Sets nullptr to hist to avoid null pointer error
  void clear();

  // Sets the collection
  void setCollection(std::shared_ptr<ParticleCollection<Particle>> particleCollection);

  //This function is necessary to change name from default in ParticleType / called in EventModule
  void changeName (std::string newName) {setName(newName);};

  // value function which is called in HistMod
  virtual std::vector<double> value() const override;

  private:
  std::shared_ptr<ParticleCollection<Particle>> collection;

  std::function<std::vector<double>(std::shared_ptr<ParticleCollection<Particle>>)> valueFunction;
};

#endif
