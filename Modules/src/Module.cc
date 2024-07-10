#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Event Counter that calls virtual process function
bool Module::processEvent()
{
  auto start = std::chrono::steady_clock::now();
  bool results = process();
  auto end = std::chrono::steady_clock::now();
  time += end - start;
  return results;
}

void Module::finalizeEvent()
{
  finalize();

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

const std::shared_ptr<FileParams> Module::getFileParams() const 
{
  return input->getFileParams();
}