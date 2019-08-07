#ifndef LRWEIGHTMODULE_HH
#define LRWEIGHTMODULE_HH

#include <string>
#include <utility>
#include <vector>

#include "TextOutputModule.hh"

#include "FileParams.hh"
#include "Pythia8/Pythia.h"

namespace reco
{
  class GenParticle;
  class Candidate;
}

class GenEventInfoProduct;

class LRWeightModule : public TextOutputModule
{
public:

  LRWeightModule(const std::string& fileName);

  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

private:

  class SumAndCount
  {
  public:
    void addNumber(double num)
    {
      sum += num;
      ++count;
    }

    double average() const
    {
      return sum / count;
    }

    void clear()
    {
      sum = 0;
      count = 0;
    }

  private:
    double sum = 0;
    int count = 0;
  };
  
  SumAndCount lrWeight;
  SumAndCount rlWeight;

  // Mutable because Pythia isn't very careful with const correctness
  mutable Pythia8::Pythia pythia;

  FileParams currentFileParams;
  bool firstTime = true;

  void writeCurrentWeights();
  std::string formattedFilename(const FileParams& params);

  std::pair<double, double> calculateWeights(const GenEventInfoProduct& genEventInfoProduct, 
					     const std::vector<reco::GenParticle>& gen, double lam, 
					     int inter) const;

  const reco::Candidate* getBosonMother(const reco::GenParticle& p) const;
  const reco::Candidate* getLeptonMother(const reco::GenParticle& p, bool second) const;
};

#endif
