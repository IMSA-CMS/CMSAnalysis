#include "CMSAnalysis/DataCollection/interface/Utility.hh"
#include <vector>
#include <numeric>
#include <iostream>

std::string Utility::substitute(std::string original, std::string target, std::string subs)
{
  size_t start_pos = 0;
  while((start_pos = original.find(target, start_pos)) != std::string::npos) {
      original.replace(start_pos, target.length(), subs);
      start_pos += subs.length(); 
  }
  return original;
}

std::string Utility::removeSpaces(std::string input) 
{
  std::string outputString;
  for(int i = 0; i < (int) input.length(); i++) {
	  if(input[i] != ' ') {
		  outputString = outputString + input[i];
	  }
	}
  return outputString;
}

int Utility::gcf(std::vector<int> nums) {
  int toReturn = 0;
  int previous = nums[0];

  for(int i = 1; static_cast<std::vector<int>::size_type>(i) < nums.size(); i++) {
    toReturn = std::gcd(previous, nums[i]);
    previous = toReturn;
  }

  return toReturn;
}