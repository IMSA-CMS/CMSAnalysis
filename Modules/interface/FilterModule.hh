#ifndef FILTERMODULE_HH
#define FILTERMODULE_HH

#include <string>
#include <map>

#include "Module.hh"
#include "InputModule.hh"

namespace edm
{
  class EventBase;
}

// This class provides a way to filter events based on some criteria
// Events can be distributed or rejected outright
class FilterModule : public Module
{
public:
  // In general, this should not be overridden again.  If it must be,
  // make sure to call the base class version at the end.
  virtual bool process() override;

  // Probably this does not need to be overridden either.  If it must be,
  // the base class version should be called at the end.
  virtual void finalize() override;

  FilterModule(std::shared_ptr<Filter> filter); 
  // Gets the (already decided) filter string for later use
  std::string getFilterString() const {return filterString;}
  
protected:
  // This is the main function of the class.
  // It creates a string to represent a classification of the event.
  // Each string will create a different set of histograms, text files, etc.
  // from the AnalysisModules.
  // Return an empty string to reject the event - that will cause the event
  // not to be recorded at all.
  // virtual std::string makeFilterString() = 0;

private:
  std::string filterString;

  std::map<std::string, int> counterMap;

  std::shared_ptr<Filter> filter;
};

#endif
