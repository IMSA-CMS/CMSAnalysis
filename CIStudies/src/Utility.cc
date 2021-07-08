#include "CIAnalysis/CIStudies/interface/Utility.hh"

void Utility::getAllCombinations(int offset, int nCombo, std::vector<int>& combination, std::vector<int>& indices, std::vector<std::vector<int>>& totalCombinations)
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
