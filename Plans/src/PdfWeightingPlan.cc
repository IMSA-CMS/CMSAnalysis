#include "CMSAnalysis/Plans/interface/PdfWeightingPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/Modules/interface/PdfAnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"

using std::make_shared;

void PdfWeightingPlan::initialize()
{
  auto& modules = getModules();
  auto histMod = make_shared<PdfAnalysisModule>();
  // Hopefully doesn't break
  modules.addAnalysisModule(histMod);
}