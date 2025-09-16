#ifndef UTILITY_HH
#define UTILITY_HH

#include <vector>
#include <string>
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

namespace Utility
{
  template <typename T>
  void getAllCombinations(int offset, int nCombo, std::vector<T>& combination, std::vector<T>& indices, std::vector<std::vector<T>>& totalCombinations);
  template <typename T>
  std::vector<std::vector<T>> getAllCombinations(int nCombo, std::vector<T>& indices);
  std::string substitute(std::string original, std::string target, std::string subs);
  std::string removeSpaces(std::string input); 
  //Intended to be used to make names for higgs channels. Input the single letter symbol of your particle, 
  //the length of the name (how many particles in your sample), and if you want to eliminate channels that mean the same thing if you have same sign pairs or opposite sign pairs
  std::vector<std::string> channelMaker(std::vector<std::string> particles, int length, bool sameSign);
  bool comparator(std::string a, std::string b);
  int gcf(std::vector<int> nums);
  // Given the name of a file that should be in bin/textfiles, gives the full path so it can be easily located
  std::string getFullPath(std::string fileName);
  std::string getBasePath();
  std::string identifyChannel(ParticleCollection<Particle> particles);

}

inline bool Utility::comparator(std::string a, std::string b) {return a<b;}

template <typename T>
inline void Utility::getAllCombinations(int offset, int nCombo, std::vector<T>& combination, std::vector<T>& indices, std::vector<std::vector<T>>& totalCombinations)
{
  // nCombo: maximum number of *things* to be combined
  // combination: vector that represents each possible combination; reset after every loop
  // indices: indices of all of the things that combinations are being take on
  // offset: which index to start on
  // totalCombinations: vector of all of the combinations
  if (nCombo == 0)
  {
    totalCombinations.push_back(combination);
    return;
  }
  for (size_t i = offset; i <= indices.size() - nCombo; ++i)
  {
    combination.push_back(indices[i]);
    getAllCombinations(i+1, nCombo-1, combination, indices, totalCombinations);
    combination.pop_back();
  }
}

template <typename T>
inline std::vector<std::vector<T>> Utility::getAllCombinations(int nCombo, std::vector<T>& indices)
{
  std::vector<std::vector<T>> totalCombinations;
  std::vector<T> combination;

  getAllCombinations(0, nCombo, combination, indices, totalCombinations);

  return totalCombinations;
}


#endif 




