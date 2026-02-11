#ifndef RUNCUT_HH
#define RUNCUT_HH

#include <memory>
#include <vector>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class Event;

class RunCut : public Filter
{
public:
  RunCut(std::string filename);

protected:
  virtual bool checkEventInternal(const Event& event, const EventInput* input) const;

private: 
  std::vector<long> runNumbers;

};


#endif
