#ifndef BIGPASTINGMODULE_HH
#define BIGPASTINGMODULE_HH

#include "HistogramOutputModule.hh"
#include "PastingModule.hh"

#include <iostream>
#include <fstream>

class PastingModule;

class BigPastingModule: public HistogramOutputModule
{
public:
  BigPastingModule(PastingModule& pastingModule);

  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

  void printCurrentHistogram();
  void clearCurrentHistogram();

private:
  PastingModule& pasting;

  bool isNewHistogram();
  std::vector<std::string> previousFileParams = {};
  std::ofstream outputFile;
  std::string outputRow;
};

#endif
