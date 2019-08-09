#include "CIAnalysis/CIStudies/interface/Module.hh"

// Static initialization
std::unordered_map<std::string, double> Module::parameters;

// Default constructor - not very elegant, hopefully it won't break things
FileParams Module::currentParams();
