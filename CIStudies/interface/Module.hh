#ifndef MODULE_HH
#define MODULE_HH

#include <string>
#include <unordered_map>

namespace edm
{
  class EventBase;
}

class Module
{
public:
  virtual bool process(const edm::EventBase& event) = 0;
  void setValue(const std::string& type, double value)
  {parameters[type] = value;}
  double getValue(const std::string& type) {return parameters[type];}

private:
  std::unordered_map<std::string, double> parameters;
  
};

#endif
