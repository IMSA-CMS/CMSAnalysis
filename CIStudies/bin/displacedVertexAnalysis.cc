#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/METModule.hh"

using std::make_shared;

Analyzer displacedVertexAnalysis()
{
  Analyzer analyzer;

  auto metMod = make_shared<METModule>();

  std::cerr << "phase one complete, initiate drone strike" << std::endl;

  // Add the filter modules to the histogram(s) created above
  // no filters for now

  // Add the histogram(s) created above to histMod
  // also a no for now

  analyzer.addProductionModule(metMod);

  return analyzer;
}
