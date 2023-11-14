#include "CMSAnalysis/Plans/interface/ModuleCollection.hh"
#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/ProductionModule.hh"

ModuleCollection::ModuleCollection() {}

std::vector<std::shared_ptr<ProductionModule>> ModuleCollection::getProductionModules() {
    return productionModules;
}

std::vector<std::shared_ptr<FilterModule>> ModuleCollection::getFilterModules() {
    return filterModules;
}

std::vector<std::shared_ptr<AnalysisModule>> ModuleCollection::getAnalysisModules() {
    return analysisModules;
}

void ModuleCollection::addProductionModule(std::shared_ptr<ProductionModule> module) {
    productionModules.push_back(module);
}

void ModuleCollection::addFilterModule(std::shared_ptr<FilterModule> module) {
    filterModules.push_back(module);
}

void ModuleCollection::addAnalysisModule(std::shared_ptr<AnalysisModule> module) {
    analysisModules.push_back(module);
}

