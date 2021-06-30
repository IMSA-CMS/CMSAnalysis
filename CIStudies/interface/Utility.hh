#ifndef UTILITY_HH
#define UTILITY_HH

#include <vector>

namespace Utility
{
  void getAllCombinations(int offset, int nCombo, std::vector<int>& combination, std::vector<int>& indices, std::vector<std::vector<int>>& totalCombinations);
}

#endif
