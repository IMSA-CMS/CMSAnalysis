#ifndef ANALYZER_HH
#define ANALYZER_HH

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>
#include "FileParams.hh"
#include "InputModule.hh"
#include "ProcessDictionary.hh"
#include "RootEventInterface.hh"

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

InputModule* getInputModule() {return input;}

const InputModule* getInputModule() const {return input;}

  // Add a production module, which is guaranteed to run before any analysis or filter
  // module (in the order added)
  void addProductionModule(std::shared_ptr<ProductionModule> module) {productionModules.push_back(module);}

  // Add a filter module, which runs after production modules and before analysis
  // modules (in the order added)
  void addFilterModule(std::shared_ptr<FilterModule> module) {filterModules.push_back(module);}

  // Add an analysis module, which runs after all production and filter modules and
  // produces output (duplicated if filter modules cause multiple paths)
  void addAnalysisModule(std::shared_ptr<AnalysisModule> module) {analysisModules.push_back(module);}
  
  void writeOutputFile(const std::string& outputFile);
  void processOneEvent(const EventInterface *eventInterface);
  void initialize();

  void printModules(){
    std::cout << "Printing Modules" << std::endl;
    for (std::shared_ptr<Module> i: getAllModules()){
      std::cout << i << ' ' << std::endl;
    }
  }

private:
  std::vector<std::shared_ptr<ProductionModule>> productionModules;
  std::vector<std::shared_ptr<FilterModule>> filterModules;
  std::vector<std::shared_ptr<AnalysisModule>> analysisModules;
  std::unordered_set<std::string> filterNames;

  int numOfEvents = 0;
  
  const EventInterface *eventInterface;
  InputModule* input;


  // Simple utility function allowing an operation to be performed on all
  // modules, regardless of type
  
  std::vector<std::shared_ptr<Module>> getAllModules() const;
};

#endif
