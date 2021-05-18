#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/DisplacedVertexModule.hh"

#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/METModule.hh"

using std::make_shared;

Analyzer displacedVertexAnalysis()
{
  Analyzer analyzer;

  auto recoMod = make_shared<RecoIdentificationModule>(5);
  auto displacedMod = make_shared<DisplacedVertexModule>(recoMod);

  // Add the histogram(s) created above to histMod
  //histMod->addHistogram(nLeptonsHist);
  //histMod->addHistogram(nElectronsHist);
  //histMod->addHistogram(nMuonsHist);

  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(displacedMod);
  
  auto metMod = make_shared<METModule>();

  std::cerr << "phase one complete, initiate drone strike" << std::endl;

  // Add the filter modules to the histogram(s) created above
  // no filters for now

  // Add the histogram(s) created above to histMod
  // also a no for now

  analyzer.addProductionModule(metMod);

  return analyzer;
}
