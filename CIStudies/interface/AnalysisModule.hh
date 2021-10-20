#ifndef ANALYSISMODULE_HH
#define ANALYSISMODULE_HH

#include <string>

#include "Module.hh"

// This is a class that creates output.
// It should go at the end of a chain of ProductionModules and FilterModules.
// You can include several of them in one Analyzer job.

class AnalysisModule : public Module
{
public:
  // This writes all relevant files.
  // It is called after finalize().
  virtual void writeAll() = 0;

  // Sets the current filter string used to group resulting objects
  // This should just be used by Analyzer::run()!
  void setFilterString(const std::string& filterString) {currentFilter = filterString;}

  // Checks to see if processing is completed
  // This can be useful for certain filtering jobs
  void doneProcessing() {isFinalStep = true;}

  // Gets the current filter string
  std::string getFilter() const {return currentFilter;}

protected:
  // Lets us know if processing has completed
  bool isDone() const {return isFinalStep;}

private:
  std::string currentFilter;
  bool isFinalStep = false;
};

#endif
