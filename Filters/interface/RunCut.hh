#ifndef RUNCUT_HH
#define RUNCUT_HH

#include <memory>
#include <set>
#include <unordered_map>

#include "CMSAnalysis/Filters/interface/Cut.hh"

class Event;

class RunCut : public Cut
{
public:
  RunCut(std::string filename);

protected:
  virtual bool checkEventInternal(const Event& event, const EventInput* input) const override;

private: 
  std::unordered_map<long, std::set<std::pair<int, int>>> runAndLumiRanges;

};


#endif
