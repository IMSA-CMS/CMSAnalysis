#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"

#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/METTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleIsolatedMuonTrigger.hh"

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
