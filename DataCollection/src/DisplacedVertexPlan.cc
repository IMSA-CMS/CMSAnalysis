#include "CMSAnalysis/DataCollection/interface/DisplacedVertexPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"
#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
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
#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"

using std::make_shared;

void DisplacedVertexPlan::setupAnalyzer()
{
  Analyzer& analyzer = getAnalyzer();

  auto dispVerMod = make_shared<DisplacedVertexModule>();

  analyzer.addProductionModule(dispVerMod);

}