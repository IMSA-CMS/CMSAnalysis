#include "CIAnalysis/CIStudies/interface/Module.hh"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Default values - not very elegant, hopefully it won't break things
FileParams Module::currentParams = FileParams();

//Event Counter that calls virtual process function
bool Module::processEvent(const edm::EventBase& event)
{
  auto fileKey = currentParams.getFileKey();
  ++eventCount[fileKey];
  process(event);

  return process(event);
}
