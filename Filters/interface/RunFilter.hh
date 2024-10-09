#ifndef RUNFILTER_HH
#define RUNFILTER_HH

#include <memory>
#include <vector>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class RunFilter : public Filter
{
public:
  void addRunNumber(long runNumber) {runNumbers.push_back(runNumber);}

protected:
  virtual std::string getFilterString(const EventInput* inputMod) const override;

private: 
  std::vector<long> runNumbers;

};


#endif
