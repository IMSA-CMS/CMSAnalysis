#include "CMSAnalysis/Modules/interface/PdfAnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>


PdfAnalysisModule::PdfAnalysisModule(int low, int high, int histBins) : HistogramOutputModule()
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
  
  //for(int i = 0; i < 100; ++i)
  //{
    //auto hist = histVec[i];
    //std::cout << "Bin1 " << hist->GetBinContent(1) << std::endl;
  //}

  auto highHist = new TH1F("high", "high", histBins, low, high);
  auto lowHist = new TH1F("low", "low", histBins, low, high);
  highHist->SetName("HighWeightedPdf");
  lowHist->SetName("LowWeightedPdf");
  defHist->SetName("DefaultHistogram");
  histVec[15]->SetName("LowHistogram");
  histVec[82]->SetName("HighHistogram");
  int count = 0;
  double lWeight = 0;
  double hWeight = 0;

  //Iterate through bins
  for(int i = 0; i < histBins; ++i)
  {
    
    std::vector<double> binRatios;
    //iterate through histograms
    for(int j = 0; j < 100; ++j)
    {
      
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
      highHist->SetBinContent(i, 0);
      lowHist->SetBinContent(i, 0);
    }
    else
    {
      std::sort(binRatios.begin(), binRatios.end());
      highHist->SetBinContent(i, binRatios[82]);
      lowHist->SetBinContent(i, binRatios[15]);
      lWeight+= binRatios[15];
      hWeight+= binRatios[82];
      count++;
    }
    histogramBinRatios.push_back(binRatios);
  }
  auto highResHist = new TH1F("highResidual", "highResidual", histBins, low, high);
  auto lowResHist = new TH1F("lowResidual", "lowResidual", histBins, low, high);
  lowResHist->SetName("LowResHistogram");
  highResHist->SetName("HighResHistogram");

  for(int i = 0; i < histBins; ++i)
  {
    auto lowBin = histVec[15]->GetBinContent(i);
    auto highBin = histVec[82]->GetBinContent(i);
    auto defBin = defHist->GetBinContent(i);
    if (defBin != 0)
    {
      lowResHist->SetBinContent(i, (defBin - lowBin)/defBin);
      highResHist->SetBinContent(i, (defBin - highBin)/defBin);
    }
    
  }
  std::cout << "writing histograms" << std::endl;
  std::cout << "Average Low Weight " << lWeight/count << std::endl;
  std::cout << "Average High Weight " << hWeight/count << std::endl;
  highHist->Write();
  lowHist->Write();
  defHist->Write();
  histVec[15]->Write();
  histVec[82]->Write();
  lowResHist->Write();
  highResHist->Write();
  
}
bool PdfAnalysisModule::process()
{

  //std::cout << "Processing" << std::endl;
  auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
  if(recoParticles.size() == 0)
  {
    //std::cout << "No Reco Particles" << std::endl;
    return true;
  }
  
  auto& particle = recoParticles[0];
  //No idea how to get particle info
  for(int i = 0; i < 100; ++i)
  {
    // 
    auto pdfWeight = particle.getInfo("pweight"+std::to_string(i));
    //std::cout << "Weight " << pdfWeight << std::endl;
    auto hist = histVec[i];
    //hist->Fill(recoInvMass, pdfWeight);
    //std::cout << "Bin1 Process " << hist->GetBinContent(1) << std::endl;
  }
  return true;
}