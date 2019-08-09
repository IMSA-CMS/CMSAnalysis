#ifndef PRODUCTIONMODULE_HH
#define PRODUCTIONMODULE_HH

#include "Module.hh"

// This class is used for intermediate, "stepping-stone" modules that perform
// tasks that are used by the AnalysisModules (or other ProductionModules).
// The class type designates it for use by Analyzer but provides no additional
// functionality.
class ProductionModule : public Module
{
public:
};

#endif
