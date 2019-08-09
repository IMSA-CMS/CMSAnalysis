#ifndef FILTERMODULE_HH
#define FILTERMODULE_HH

#include <string>

#include "Module.hh"

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
  // make sure to call the base class version at the end
  virtual bool process(const edm::EventBase& event) override;

  // Gets the (already decided) filter string for later use
  std::string getFilterString() const {return filterString;}
  
protected:
  // This is the main function of the class.
  // It creates a string to represent a classification of the event.
  // Each string will create a different set of histograms, text files, etc.
  // from the AnalysisModules.
  // Return an empty string to reject the event - that will cause the event
  // not to be recorded at all.
  virtual std::string makeFilterString(const edm::EventBase& event) = 0;

private:
  std::string filterString;
};

#endif
