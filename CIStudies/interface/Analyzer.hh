#ifndef ANALYZER_HH
#define ANALYZER_HH

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "FileParams.hh"

class AnalysisModule;
class FilterModule;
class Module;
class ProductionModule;

// A general class to run an analysis, consisting of many sequential modules
class Analyzer
{
public:
  // Add a production module, which is guaranteed to run before any analysis or filter
  // module (in the order added)
  void addProductionModule(std::shared_ptr<ProductionModule> module) 
  {productionModules.push_back(module);}
  // Add a filter module, which runs after production modules and before analysis
  // modules (in the order added)
  void addFilterModule(std::shared_ptr<FilterModule> module)
  {filterModules.push_back(module);}
  // Add an analysis module, which runs after all production and filter modules and
  // produces output (duplicated if filter modules cause multiple paths)
  void addAnalysisModule(std::shared_ptr<AnalysisModule> module) 
  {analysisModules.push_back(module);}

  // Run the analysis for a given configuration file, a Root output file,
  // and an optional parameter to output with a certain event frequency
  void run(const std::string& configFile, const std::string& outputFile, 
	   int outputEvery = 0, int nFiles = -1);

private:
  std::vector<std::shared_ptr<ProductionModule>> productionModules;
  std::vector<std::shared_ptr<FilterModule>> filterModules;
  std::vector<std::shared_ptr<AnalysisModule>> analysisModules;

  // Parse one line of the the configuration file
  std::vector<std::string> parseLine(std::ifstream& txtFile) const;
  // Find all input files from the configuration files, stored as FileParams objects
  std::vector<FileParams> inputFiles(const std::string& txtFile) const;
  // Simple utility function allowing an operation to be performed on all
  // moduels, regardless of type
  std::vector<std::shared_ptr<Module>> getAllModules() const;
};

#endif
