#ifndef ANALYZER_HH
#define ANALYZER_HH

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class AnalysisModule;
class FilterModule;
class Module;
class ProductionModule;

class Analyzer
{
public:
  void addProductionModule(ProductionModule* module) 
  {productionModules.push_back(module);}
  void addFilterModule(FilterModule* module)
  {filterModules.push_back(module);}
  void addAnalysisModule(AnalysisModule* module) 
  {analysisModules.push_back(module);}

  void run(const std::string& configFile, const std::string& outputFile, int outputEvery = 0);

private:
  std::vector<ProductionModule*> productionModules;
  std::vector<FilterModule*> filterModules;
  std::vector<AnalysisModule*> analysisModules;

  std::vector<std::string> parseLine(std::ifstream& txtFile) const;
  std::vector<std::vector<std::string>> inputFiles(const std::string& txtFile, std::string& particle1) const;
  std::vector<std::vector<std::string>> getFiles(const std::vector<std::vector<std::string>>& inputs) const;
  std::vector<Module*> getAllModules() const;
};

#endif
