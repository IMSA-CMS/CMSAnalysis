#include "CMSAnalysis/Modules/interface/PdfAnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>


PdfAnalysisModule::PdfAnalysisModule(int low, int high, int histBins) :
  HistogramOutputModule()
{
  this->low = low;
  this->high = high;
  this->histBins = histBins;
}


void PdfAnalysisModule::initialize()
{  
  
  bool oldVal = TH1::GetDefaultSumw2();
  defHist = new TH1F("Unweighted", "Unweighted", histBins, low, high);
  TH1::SetDefaultSumw2(kTRUE);
  for (int i = 0; i < 100; ++i){
    
    std::string s = "PDFWeighted" + std::to_string(i);
    const char* thName = s.c_str();
    TH1* newHist = new TH1F(thName, thName, histBins, low, high);
    histVec.push_back(newHist);
    
  }
  TH1::SetDefaultSumw2(oldVal);
}

void PdfAnalysisModule::finalize()
{
  std::cout << "PdfAnalysisModule Finalize Started" << std::endl;
  for(int i = 0; i < 100; ++i){
    auto hist = histVec[i];
    std::vector<double> binRatios;
    std::cout << "Filling RatiosHistogram " << i << std::endl;
    for(int j = 0; j < histBins; ++j){
      std::cout << "At Bin: " << j << std::endl;
      auto binContent = hist->GetBinContent(j);
      double binRatio = binContent/(defHist->GetBinContent(j));
      binRatios.push_back(binRatio);
      std::cout << "Added Ratio" << std::endl;
      //Some problem here as well 
   
    }
    std::sort(binRatios.begin(), binRatios.end());
    histogramBinRatios.push_back(binRatios);
    // #16-#83

  }
  AnalysisModule::finalize();
}

bool PdfAnalysisModule::process()
{
  
  auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
  //auto recoCS = recoParticles.getCollinsSoper();
  auto recoInvMass = recoParticles.getInvariantMass();
  defHist->Fill(recoInvMass);
  auto& particle = recoParticles[0];
  //No idea how to get particle info
  for(int i = 0; i < 100; ++i){
    // 
    auto pdfWeight = particle.getInfo("PDFWeight"+std::to_string(i));
    auto hist = histVec[i];
    hist->Fill(recoInvMass, pdfWeight);
  }
  std::cout << "PdfAnalysisModule Process Ended" << std::endl;
  return true;
}
