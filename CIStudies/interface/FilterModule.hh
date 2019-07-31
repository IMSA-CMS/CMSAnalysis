#ifndef FILTERMODULE_HH
#define FILTERMODULE_HH

#include <string>

#include "Module.hh"

namespace edm
{
  class EventBase;
}

class FilterModule : public Module
{
public:
  virtual bool process(const edm::EventBase& event) override;

  std::string getFilterString() const {return filterString;}
  
protected:
  // Return empty string to reject event
  virtual std::string makeFilterString(const edm::EventBase& event) = 0;

private:
  std::string filterString;
};

#endif
