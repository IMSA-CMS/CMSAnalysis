#ifndef MODULE_HH
#define MODULE_HH

#include <string>
#include <unordered_map>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/FileParams.hh"

namespace edm
{
  class EventBase;
}

class EventInput;

// The base class for all modules used in the Analyzer framework
class Module
{
public:
  virtual ~Module();

  // This is called once before any files are accessed
  // Used to set up objects such as histograms
  virtual void initialize() {}

  // This is called once after all files are processed
  // Used for cleanup and to create final graphs and plots
  virtual void finalize() {}

  // These allow the program to set global variables to values
  // They are stored in a map that can be accessed by any module
  // Note that they are static and not per-module
  static void setValue(const std::string& type, double value)
  {parameters[type] = value;}
  static double getValue(const std::string& type) {return parameters[type];}
  int getEventCount(const std::string massBin) {return eventCount[massBin];}

  bool processEvent();
  virtual void setInput(const EventInput* iInput) {input = iInput;}
  const EventInput* getInput() const {return input;}

  // Writes text to screen and to Root file
  // par is the text itself while name is the name of the variable in the Root file
  void writeText(std::string par, std::string name) const;
  const int getInputPublic() const {
    if(input){return 1;}
    return 0;
    }

protected:
  // This is called once per event, with the event object passed in.
  // Most of the work should happen here.
  // Return false to stop execution on this event, which should only be done
  // if you are deliberately filtering events.
  // In most cases, you should return true.
  virtual bool process() = 0;


private:
  static std::unordered_map<std::string, double> parameters;
  std::unordered_map<std::string, int> eventCount; 
  const EventInput* input = nullptr;

};

#endif
