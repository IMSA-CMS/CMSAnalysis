#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/DisplacedVertexModule.hh"


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
  
  return analyzer;
}
