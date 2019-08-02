#include "CIAnalysis/CIStudies/interface/Module.hh"

std::unordered_map<std::string, double> Module::parameters;

// Default values - not very elegant, hopefully it won't break things
FileParams Module::currentParams("2016", "LL", "Con", "300", "16", "Mu");
