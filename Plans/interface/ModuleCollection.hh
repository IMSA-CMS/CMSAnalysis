#ifndef MODULECOLLECTION_HH
#define MODULECOLLECTION_HH

#include <fstream>
#include <vector>

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/ProductionModule.hh"


class ProductionModule;
class FilterModule;
class AnalysisModule;


class ModuleCollection
{
public:
ModuleCollection();
// Add a production module, which is guaranteed to run before any analysis or filter
  // module (in the order added)
  void addProductionModule(std::shared_ptr<ProductionModule> module);

  // Add a filter module, which runs after production modules and before analysis
  // modules (in the order added)
  void addFilterModule(std::shared_ptr<FilterModule> module);

  // Add an analysis module, which runs after all production and filter modules and
  // produces output (duplicated if filter modules cause multiple paths)
  void addAnalysisModule(std::shared_ptr<AnalysisModule> module);

  std::vector<std::shared_ptr<ProductionModule>> getProductionModules();
  std::vector<std::shared_ptr<FilterModule>> getFilterModules();
  std::vector<std::shared_ptr<AnalysisModule>> getAnalysisModules();

  
private:
	std::vector<std::shared_ptr<ProductionModule>> productionModules;
	std::vector<std::shared_ptr<FilterModule>> filterModules;
	std::vector<std::shared_ptr<AnalysisModule>> analysisModules;
};
#endif