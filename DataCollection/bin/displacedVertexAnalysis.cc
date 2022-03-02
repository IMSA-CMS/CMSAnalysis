#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"

#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

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
#include "CIAnalysis/CIStudies/interface/DisplacedVertexModule.hh"

using std::make_shared;

Analyzer displacedVertexAnalysis()
{
  Analyzer analyzer;

  auto dispVerMod = make_shared<DisplacedVertexModule>();

  analyzer.addProductionModule(dispVerMod);
  
  return analyzer;
}
