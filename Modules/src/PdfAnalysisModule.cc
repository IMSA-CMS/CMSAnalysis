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
  
  for(int i = 0; i < 100; ++i)
  {
    auto hist = histVec[i];
    //std::cout << "Bin1 " << hist->GetBinContent(1) << std::endl;
  }
  auto highHist = new TH1F("high", "high", histBins, low, high);
  auto lowHist = new TH1F("low", "low", histBins, low, high);
  highHist->SetName("HighWeightedPdf");
  lowHist->SetName("LowWeightedPdf");
  //Iterate through bins
  for(int i = 0; i < histBins; ++i){
    
    std::vector<double> binRatios;
    //iterate through histograms
    for(int j = 0; j < 100; ++j){
      
      auto hist = histVec[j];
      //Iterate through bins of each histogram
      
      auto binContent = hist->GetBinContent(i);  
      if(!binContent == 0 || !defHist->GetBinContent(i) == 0)
      {
      double binRatio = binContent/(defHist->GetBinContent(i));
      binRatios.push_back(binRatio);
      }
      //Compute Ratios
    }
    if(binRatios.size() == 0)
    {
      //std::cout << "no ratios for bin " << i << std::endl;
      highHist->SetBinContent(i, -1);
      lowHist->SetBinContent(i, -1);
    }
    else{
    std::sort(binRatios.begin(), binRatios.end());
  //  std::cout << "Ratios for Bin " << i << std::endl;
    //for(double ratio : binRatios)
   // {
  //    std::cout << ratio << " ";
   // }
  //  std::cout << std::endl;
    highHist->SetBinContent(i, binRatios[82]);
  //  std::cout << "lowValue " << binRatios[15] << std::endl;
    lowHist->SetBinContent(i, binRatios[15]);
  //  std::cout << "highValue " << binRatios[82] << std::endl;
  // blah blah blah highHist->SetBinContent(i, 78.2);
    }
    histogramBinRatios.push_back(binRatios);
    //Add ratio set of weighted histogram to vector
    // #16-#83

  }
  std::cout << "writing histograms" << std::endl;
  highHist->Write();
  lowHist->Write();
  AnalysisModule::finalize();
}

bool PdfAnalysisModule::process()
{

  std::cout << "Processing" << std::endl;
  auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
  if(recoParticles.size() == 0)
  {
    std::cout << "No Reco Particles" << std::endl;
    return true;
  }
  auto recoInvMass = recoParticles.getInvariantMass();
  defHist->Fill(recoInvMass, 1.0);
  auto& particle = recoParticles[0];
  for(int i = 0; i < 100; ++i)
  {
    // 
    auto pdfWeight = particle.getInfo("pweight"+std::to_string(i));
<<<<<<< HEAD
=======
    //std::cout << "Weight " << pdfWeight << std::endl;
>>>>>>> 1ec330c7da40d961468ba5742976abbf02026714
    auto hist = histVec[i];
    hist->Fill(recoInvMass, pdfWeight);
    //std::cout << "Bin1 Process " << hist->GetBinContent(1) << std::endl;
  }
  return true;
}
