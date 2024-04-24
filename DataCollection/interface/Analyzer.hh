#ifndef ANALYZER_HH
#define ANALYZER_HH

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "ProcessDictionary.hh"
#include "RootEventInterface.hh"
#include "CMSAnalysis/Plans/interface/ModuleCollection.hh"

//#include "ProductionModule.hh"

class AnalysisModule;
class FilterModule;
class Module;
class ProductionModule;
class EventInterface;

namespace edm
{
  class Event;
}

// A general class to run an analysis, consisting of many sequential modules
class Analyzer
{
public:
  Analyzer();
  Analyzer(const Analyzer& analyzer1);
  ~Analyzer();

EventInput* getEventInput() {return input;}

const EventInput* getEventInput() const {return input;}

  // Add a production module, which is guaranteed to run before any analysis or filter
  // module (in the order added)
  void addProductionModule(std::shared_ptr<ProductionModule> module);

  // Add a filter module, which runs after production modules and before analysis
  // modules (in the order added)
  void addFilterModule(std::shared_ptr<FilterModule> module);

  // Add an analysis module, which runs after all production and filter modules and
  // produces output (duplicated if filter modules cause multiple paths)
  void addAnalysisModule(std::shared_ptr<AnalysisModule> module);
  
  void writeOutputFile();
  void processOneEvent(const EventInterface *eventInterface);
  void initialize(const std::string& outputFile);
  bool checkModuleDependencies(std::shared_ptr<Module> module);
  void addModules(ModuleCollection modules);
 

private:
  std::vector<std::shared_ptr<ProductionModule>> productionModules; //= ModuleCollection.getProductionModules();
  std::vector<std::shared_ptr<FilterModule>> filterModules; //= ModuleCollection.getFilterModules();
  std::vector<std::shared_ptr<AnalysisModule>> analysisModules;// = ModuleCollection.getAnalysisModules();
  std::unordered_set<std::string> filterNames;
  std::unordered_map<std::string, TDirectory*> filterDirectories;
  TFile* outputRootFile = nullptr;

  int numOfEvents = 0;


  const EventInterface *eventInterface;
  EventInput* input;


  // Simple utility function allowing an operation to be performed on all
  // modules, regardless of type
  
  std::vector<std::shared_ptr<Module>> getAllModules() const;
};

#endif
