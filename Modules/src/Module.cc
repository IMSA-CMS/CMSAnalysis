#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Event Counter that calls virtual process function
bool Module::processEvent()
{
  return process();
}

Module::~Module() {}

void Module::writeText(std::string par, std::string name) const
{
  TDisplayText *printPar = new TDisplayText(par.c_str());
  printPar->Write(name.c_str());
  std::cout << name << " : " << par << std::endl;
}

void Module::addRequiredModule(std::shared_ptr<Module> module)
{
  if (std::find(dependencies.begin(), dependencies.end(), module) == dependencies.end())
  {
    dependencies.push_back(module);
  }
}