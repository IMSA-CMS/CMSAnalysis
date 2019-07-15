#ifndef ANALYZER_HH
#define ANALYZER_HH

#include <fstream>
#include <string>
#include <vector>

class ProductionModule;
class AnalysisModule;

class Analyzer
{
public:
  void addProductionModule(ProductionModule* module) 
  {productionModules.push_back(module);}
  void addAnalysisModule(AnalysisModule* module) 
  {analysisModules.push_back(module);}

  void run(const std::string& configFile, const std::string& outputFile, int outputEvery = 0);

private:
  std::vector<ProductionModule*> productionModules;
  std::vector<AnalysisModule*> analysisModules;

  std::vector<std::string> parseLine(std::ifstream& txtFile);
  std::vector<std::vector<std::string>> inputFiles(const std::string& txtFile, std::string& particle1);
  std::vector<std::vector<std::string>> getFiles(const std::vector<std::vector<std::string>>& inputs);
};

#endif
