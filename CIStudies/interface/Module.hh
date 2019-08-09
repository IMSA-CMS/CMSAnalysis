#ifndef MODULE_HH
#define MODULE_HH

#include <string>
#include <unordered_map>

#include "FileParams.hh"

namespace edm
{
  class EventBase;
}

// The base class for all modules used in the Analyzer framework
class Module
{
public:
  virtual ~Module() {}

  // This is called once before any files are accessed
  // Used to set up objects such as histograms
  virtual void initialize() {}

  // This is called once after all files are processed
  // Used for cleanup and to create final graphs and plots
  virtual void finalize() {}

  // This is called once per event, with the event object passed in.
  // Most of the work should happen here.
  // Return false to stop execution on this event, which should only be done
  // if you are deliberately filtering events.
  // In most cases, you should return true.
  virtual bool process(const edm::EventBase& event) = 0;

  // These allow the program to set global variables to values
  // They are stored in a map that can be accessed by any module
  // Note that they are static and not per-module
  static void setValue(const std::string& type, double value)
  {parameters[type] = value;}
  static double getValue(const std::string& type) {return parameters[type];}

  // This sets the FileParams object that the current event is using
  // Note that this is (dangerously) set to a default value before events are accessed
  // So it is generally not safe to access this except during process()
  // The set function should only be called by Analyzer!
  static void setFileParams(FileParams params) {currentParams = params;}
  static FileParams getFileParams() {return currentParams;}

private:
  static std::unordered_map<std::string, double> parameters;
  
  static FileParams currentParams;
};

#endif
