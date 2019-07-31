#include "CIAnalysis/CIStudies/interface/AnalysisModule.hh"

#include <stdexcept>

#include "TH1.h"

void AnalysisModule::finalize()
{
  for (auto& entry : objects)
    {
      entry.second->Write();
    }

  Module::finalize();
}

void AnalysisModule::makeHistogram(const std::string& name, const std::string& title, int nbins, double min, double max)
{
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void AnalysisModule::fillHistogram(const std::string& name, double number)
{
  if (objects.find(name) != objects.end())
    {
      getHistogram(name)->Fill(number);
    }
  else
    {
      throw std::runtime_error("Histogram " + name + ", called in AnalysisModule::fillHistogram(), does not exist!");
    }
}
