#ifndef ANALYSISMODULE_HH
#define ANALYSISMODULE_HH

#include <TFile.h>
#include <string>

#include "CMSAnalysis/Modules/interface/Module.hh"


// This is a class that creates output.
// It should go at the end of a chain of ProductionModules and FilterModules.
// You can include several of them in one Analyzer job.
class AnalysisModule : public Module
{
public:
  // This writes all relevant files.
  // It is called after finalize().

  virtual void writeObjects(TFile *outFile) {}

  // Sets the current filter string used to group resulting objects
  // This should just be used by Analyzer::run()!
  void setFilterStrings(std::vector<std::string> filterStrings) {currentFilters = filterStrings;}

  // Checks to see if processing is completed
  // This can be useful for certain filtering jobs
  void doneProcessing() {isFinalStep = true;}

  std::string getFilter() const
  {
      std::string filter = "";
      for (auto part : currentFilters) {
          filter += part;
          filter += "_";
      }
      return filter;
  };

  // Gets the current filter strings
  std::vector<std::string> getFilters() const {return currentFilters;}

  virtual std::string getName() override {return "AnalysisModule";}

protected:
  // Lets us know if processing has completed
  bool isDone() const {return isFinalStep;}

private:
  std::vector<std::string> currentFilters;
  bool isFinalStep = false;
};

#endif
