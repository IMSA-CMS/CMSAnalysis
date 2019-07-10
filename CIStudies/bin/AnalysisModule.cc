#include "AnalysisModule.hh"

#include "TH1.h"

AnalysisModule::~AnalysisModule()
{
  for (auto& entry : histograms)
    {
      delete entry.second;
    }
}

void AnalysisModule::initialize()
{
  createHistograms();
}

void AnalysisModule::writeAllHistograms()
{
  for (auto& entry : histograms)
    {
      entry.second->Write();
    }
}

void AnalysisModule::makeHistogram(const std::string& name, const std::string& title, int nbins, int min, int max)
{
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  histograms.insert({name, newHist});
}
