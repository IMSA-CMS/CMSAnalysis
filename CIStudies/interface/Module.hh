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
  virtual ~Module() {}

  virtual void initialize() {}
  virtual void finalize() {}

  virtual bool process(const edm::EventBase& event) = 0;
  static void setValue(const std::string& type, double value)
  {parameters[type] = value;}
  static double getValue(const std::string& type) {return parameters[type];}

private:
  static std::unordered_map<std::string, double> parameters;
  
};

#endif
