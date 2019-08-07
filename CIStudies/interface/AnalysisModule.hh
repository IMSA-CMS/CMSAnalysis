#ifndef ANALYSISMODULE_HH
#define ANALYSISMODULE_HH

#include <map>
#include <string>

#include "TH1.h"

#include "Module.hh"

class TH1;

class AnalysisModule : public Module
{
public:
  virtual void writeAll() = 0;

  // This should just be used by Analyzer::run()
  void setFilterString(const std::string& filterString) {currentFilter = filterString;}

  void doneProcessing() {isFinalStep = true;}

  std::string getFilter() const {return currentFilter;}
  bool getFinalStep() const {return isFinalStep;}

private:
  std::string currentFilter;
  bool isFinalStep = false;
};

#endif
