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
	int nBins = 1000;
	int low = 0;
	int high = 1000;
	std::cout << "Initialiing PdfAnalysisModule" << std::endl;

	bool oldVal = TH1::GetDefaultSumw2();
	TH1::SetDefaultSumw2(kTRUE);
	vector<TH1*> histVec;
	for (int i = 0; i < 101; ++i){
	TH1* newHist = new TH1F("UnWeighted", "UnWeighted", nBins, low, high);
   histVec.push_back(newHist);  
}
TH1::SetDefaultSumw2(oldVal);

for (int i = 0 i < 101; ++1){
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
	for(int i=0; i < 100; ++i){
		vector<double> binRatios;
		auto hist = histVec[i];
		for(int j = 0 j < 1000; ++j){
			auto binContent = hist->GetBinContent(j);
			double binRatio = binContent/(defHist->GetBinContent(j));
			binRatios[i].push_back(binRatio);
		}
		std::sort(histogramBinRatios[i].begin(), histogramBinRatios[i].end());+
		histogramBinRatios.push_back(binRatios);
	}
  AnalysisModule::finalize();
}

bool AFBModule::process()
{
	auto genParticles = getInput()->getLeptons(EventInput::RecoLevel::GenSim);
//auto genSimCS = genParticles.getCollinsSoper();
	auto genSimInvMass = genParticles.getInvariantMass();
	defHist->Fill(recoInvMass, 1.0);
	auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
//auto recoCS = recoParticles.getCollinsSoper();
	auto recoInvMass = recoParticles.getInvariantMass();
for(int i = 1; i < 101; ++i){
   auto pdfWeight = genParticles.getInfo "PDFWeight"+std::to_string(i);
   auto hist = histVec[i];
   hist->Fill(recoInvMass, pdfWeight);
}

return true;
}

