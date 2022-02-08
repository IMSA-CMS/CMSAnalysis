#ifndef MATCHINGPAIRCOLLECTION_HH
#define MATCHINGPAIRCOLLECTION_HH

#include "MatchingPair.hh"
#include "ParticleCollection.hh"

#include <vector>

// MatchingPairCollection contains a vector of the MatchingPairs for each event
class MatchingPairCollection
{
public:
  void addMatchingPair(const MatchingPair pair) {matchingPairCollection.push_back(pair);}
  
  const std::vector<MatchingPair>& getPairs() const {return matchingPairCollection;}
  ParticleCollection getGenParticles() const;
  ParticleCollection getRecoParticles() const;
  double getSize() const {return matchingPairCollection.size();}
  void clear() {matchingPairCollection.clear();}

private:
  std::vector<MatchingPair> matchingPairCollection;
};

#endif
