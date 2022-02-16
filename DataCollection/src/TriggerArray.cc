#ifndef TRIGGERARRAY_CC
#define TRIGGERARRAY_CC

#include "CMSAnalysis/DataCollection/interface/TriggerArray.hh"

template <class... Args> TriggerArray<Args...>::TriggerArray(std::vector<std::string> iNames) :
  names(iNames)
{}

template <class... Args> std::map<std::string, bool> TriggerArray<Args...>::checkEvent(Args&&... args)
{
  std::map<std::string, bool> data = checkTrigger(args...);
  auto iter = data.begin();

  while (iter != data.end())
  {
    if (triggerData.find(iter->first) == triggerData.end())
    {
      // if not in there, add a log to the map, make value 1 if it passed
      triggerData.insert({iter->first, {(int)iter->second, 1}}); 
    }
    else
    {
      // if in there, just increment 1 if it passed
      triggerData[iter->first].passed += (int)iter->second;
      ++triggerData[iter->first].total;
    }
    iter++;
  }

  return data;
}

template <class... Args> std::map<std::string, double> TriggerArray<Args...>::passRatios() const
{
  // Returns a map continaing the ratio of each trigger
  std::map<std::string, double> ratios;
  auto iter = triggerData.begin();

  while (iter != triggerData.end())
  {
    ratios.insert({iter->first, static_cast<double>(iter->second.passed) / (iter->second.total)});
    iter++;
  }

  return ratios;
}

template <class... Args> void TriggerArray<Args...>::printTriggerResults()
{
  // Prints out a summary of the results
  std::cout << std::endl;
  auto iter = triggerData.begin();
  while (iter != triggerData.end())
  {
    std::cout << iter->first << " : Passed " << iter->second.passed << " : Total " << iter->second.total << " : Ratio "
        << static_cast<double>(iter->second.passed) / (iter->second.total) << "\n";
    iter++;
  }
  std::cout << std::endl;
}

#endif