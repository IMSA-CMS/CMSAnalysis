#include "CMSAnalysis/DataCollection/interface/Module.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"
//#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Event Counter that calls virtual process function
bool Module::processEvent()
{
  std::cout << "Made it to module process\n";

  return process();
}

Module::~Module() {}

void Module::writeText(std::string par, std::string name) const
{
  TDisplayText *printPar = new TDisplayText(par.c_str());
  printPar->Write(name.c_str());
  std::cout << name << " : " << par << std::endl;
}