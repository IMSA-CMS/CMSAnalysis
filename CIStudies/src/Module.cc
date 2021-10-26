#include "CIAnalysis/CIStudies/interface/Module.hh"
#include "CIAnalysis/CIStudies/interface/TDisplayText.h"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Default values - not very elegant, hopefully it won't break things
FileParams Module::currentParams = FileParams();

//Event Counter that calls virtual process function
bool Module::processEvent(const edm::EventBase& event)
{
  auto fileKey = currentParams.getFileKey();
  ++eventCount[fileKey];

  return process(event);
}

void Module::writeText(std::string par, std::string name) const
{
  TDisplayText *printPar = new TDisplayText(par.c_str());

  printPar->Write(name.c_str());
  
  std::cout << name << " : " << par << std::endl;
}