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
  for(int i = 0; i < 100; ++i){
    auto hist = histVec[i];
    std::vector<double> binRatios;
    for(int j = 0; j < histBins; ++j){
      auto binContent = hist->GetBinContent(j);
      double binRatio = binContent/(defHist->GetBinContent(j));
      binRatios.push_back(binRatio);
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
  if(recoParticles.size() == 0)
  {
    std::cout << "No Reco Particles" << std::endl;
    return true;
  }
  auto recoInvMass = recoParticles.getInvariantMass();
  defHist->Fill(recoInvMass);
  auto& particle = recoParticles[0];
  for(int i = 0; i < 100; ++i)
  {
    // 
    auto pdfWeight = particle.getInfo("pweight"+std::to_string(i));
    auto hist = histVec[i];
    hist->Fill(recoInvMass, pdfWeight);
  }
  return true;
}
