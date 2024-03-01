#include "CMSAnalysis/Modules/interface/AFBModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>

AFBModule::AFBModule(int minMass, int maxMass, int massInterval) :
  HistogramOutputModule(),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}

void AFBModule::initialize()
{  
	int nBins=1000;
	int low=0;
	int high=1000; 
	TH1* newHist = new TH1F(“Name”, “Title”, nBins, low, high);
	vector<TH1*> histograms = new vector<TH1*>

for (int i = 1 i < 101; ++1){
	makeHistogram("PDFWeighted"+std::get_string(i), "PDFWeighted"+std::getstring(i), histBins, minMassCut, maxMassCut);
}

  bool oldVal = TH1::GetDefaultSumw2();
  TH1::SetDefaultSumw2(kTRUE);

  makeHistogram("GenSimForward", "GenSimForward", histBins, minMassCut, maxMassCut);
  makeHistogram("GenSimBackward", "GenSimBackward", histBins, minMassCut, maxMassCut);


  makeHistogram("RecoForward", "RecoForward", histBins, minMassCut, maxMassCut);
  makeHistogram("RecoBackward", "RecoBackward", histBins, minMassCut, maxMassCut);

  TH1::SetDefaultSumw2(oldVal);
}

void AFBModule::finalize()
{
  AnalysisModule::finalize();
}

bool AFBModule::process()
{
  auto genParticles = getInput()->getLeptons(EventInput::RecoLevel::GenSim);
  auto genSimCS = genParticles.getCollinsSoper();
  auto genSimInvMass = genParticles.getInvariantMass();

  auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
  auto recoCS = recoParticles.getCollinsSoper();
  auto recoInvMass = recoParticles.getInvariantMass();

  fillFB("GenSim", genSimCS, genSimInvMass);
  fillFB("Reco", recoCS, recoInvMass);

  return true;
}

