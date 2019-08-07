#ifndef MODULE_HH
#define MODULE_HH

#include <string>
#include <unordered_map>

#include "FileParams.hh"

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

  static void setFileParams(FileParams params) {currentParams = params;}
  static FileParams getFileParams() {return currentParams;}

private:
  static std::unordered_map<std::string, double> parameters;
  
  static FileParams currentParams;
};

#endif
