#ifndef UTILITY_HH
#define UTILITY_HH

#include <vector>

namespace Utility
{
  template <typename T>
  void getAllCombinations(int offset, int nCombo, std::vector<T>& combination, std::vector<T>& indices, std::vector<std::vector<T>>& totalCombinations);
  template <typename T>
  std::vector<std::vector<T>> getAllCombinations(int nCombo, std::vector<T>& indices);
}

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
