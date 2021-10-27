#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/DisplacedVertexModule.hh"

#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/METModule.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/METTrigger.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/SingleElectronTrigger.hh"
#include "CIAnalysis/CIStudies/interface/DoubleElectronTrigger.hh"
#include "CIAnalysis/CIStudies/interface/SingleIsolatedMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/DoubleIsolatedMuonTrigger.hh"

using std::make_shared;

Analyzer displacedVertexAnalysis()
{
  Analyzer analyzer;

  auto metMod = make_shared<METModule>();
  auto triggerMod = make_shared<TriggerModule>();

  // Initialize triggers
  auto metTrigger = make_shared<METTrigger>(metMod, 120);
  auto singleMuonTrigger = make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = make_shared<DoubleMuonTrigger>(37, 27);
  auto tripleMuonTrigger = make_shared<TripleMuonTrigger>(10, 5, 5);
  auto singleElectronTrigger = make_shared<SingleElectronTrigger>(28);
  auto doubleElectronTrigger = make_shared<DoubleElectronTrigger>(25, 25);
  auto singleIsolatedMuonTrigger = make_shared<SingleIsolatedMuonTrigger>(24);
  auto doubleIsolatedMuonTrigger = make_shared<DoubleIsolatedMuonTrigger>(17, 8);


  // Add Triggers to the TriggerModule
  triggerMod->addTrigger(metTrigger);
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);
  triggerMod->addTrigger(singleElectronTrigger);
  triggerMod->addTrigger(doubleElectronTrigger);
  triggerMod->addTrigger(singleIsolatedMuonTrigger);
  triggerMod->addTrigger(doubleIsolatedMuonTrigger);

  // Add the filter modules to the histogram(s) created above
  // no filters for now

  // Add the histogram(s) created above to histMod
  // also a no for now
  analyzer.addProductionModule(metMod);
  analyzer.addProductionModule(triggerMod);

  return analyzer;
}
