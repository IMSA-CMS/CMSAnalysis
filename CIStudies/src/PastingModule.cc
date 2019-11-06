#include "CIAnalysis/CIStudies/interface/PastingModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include "TH1.h"

PastingModule::PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, int minMass, int maxMass) :
  genSim(genSimModule),
  reco(recoModule),
  minMassCut(minMass),
  maxMassCut(maxMass)
{
}

void PastingModule::initialize()
{
  makeHistogram("GenSim Invariant Mass Pasted", "Gen Sim Invariant Mass Pasted", histBins, minMassCut, maxMassCut);
  makeHistogram("Reco Invariant Mass Pasted", "Reco Invariant Mass Pasted", histBins, minMassCut, maxMassCut);
}

bool PastingModule::process(const edm::EventBase& event)
{
  std::string massBin = getFileParams().getMassRange();
  
  if (isNewMassBin(massBin))
    {
      massBins.push_back(massBin);
      makeHistogram("GenSim" + massBin, "GenSim" + massBin, histBins, minMassCut, maxMassCut);
      makeHistogram("Reco" + massBin, "Reco" + massBin, histBins, minMassCut, maxMassCut);
    }

  auto genParticles = genSim.getGenParticles();
  auto genSimInv = genParticles.getInvariantMass();

  auto recoParticles = reco.getRecoCandidates();
  auto recoInv = recoParticles.getInvariantMass();

  fillHistogram("GenSim" + massBin, genSimInv);  
  fillHistogram("Reco" + massBin, recoInv);
  return true;
}

void PastingModule::finalize()
{
  //2016 muons

  //2016 and 2017 muons
  //std::vector<double> fileSizes = {50000, 50000, 50000, 50000};

  //2016 ADD - Lambda: 5
  std::vector<double> fileSizes = {96700};

  //2017 ADD - Lambda: 5
  //std::vector<double> fileSizes = {150000, 150000, 150000, 150000};

  //2016 electrons
  //std::vector<double> fileSizes = {49039, 50000, 50000, 48816};

  //2017 electrons
  //std::vector<double> fileSizes = {44000, 50000, 50000, 48000};

  for (int i = 0; i < static_cast<int>(massBins.size()); ++i)
    {
      auto massBin = massBins[i];
      
      std::string nextMassBin;

      if (i < static_cast<int>(massBins.size()) - 1)
	{
	  nextMassBin = massBins[i+1];
	}
      else 
	{
	  nextMassBin = "M5000";
	}
      
      // int massBinUpperCutoff = std::stoi(nextMassBin.substr(1));

      // std::cout << massBinUpperCutoff << std::endl;

      // int topBin = getHistogram("GenSim" + massBin)->GetXaxis()->FindBin(massBinUpperCutoff);

      // std::cout << topBin << std::endl;

      // for (int j = topBin; j < histBins; ++j)
      // 	{
      // 	  getHistogram("GenSim" + massBin)->SetBinContent(j,0);
      // 	  getHistogram("Reco" + massBin)->SetBinContent(j,0);
      // 	}

      // getHistogram("GenSim" + massBin)->Scale(getHistogram("GenSim" + massBin)->Integral() / getHistogram("GenSim" + massBins[0])->Integral());
      // getHistogram("Reco" + massBin)->Scale(getHistogram("Reco" + massBin)->Integral() / getHistogram("GenSim" + massBins[0])->Integral());

      getHistogram("GenSim" + massBin)->Scale(1 / fileSizes[i]);
      getHistogram("Reco" + massBin)->Scale(1 / fileSizes[i]);
    }

  // CI 2016 Muon Weights

  // getHistogram("GenSimM300")->Scale(.579800);
  // getHistogram("GenSimM800")->Scale(.019020);
  // getHistogram("GenSimM1300")->Scale(.003909);
  // getHistogram("GenSimM2000")->Scale(.001052);
  
  // getHistogram("RecoM300")->Scale(.579800);
  // getHistogram("RecoM800")->Scale(.019020);
  // getHistogram("RecoM1300")->Scale(.003909);
  // getHistogram("RecoM2000")->Scale(.001052);

  // CI 2017 Muon Weights

  // getHistogram("GenSimM300")->Scale(.656100);
  // getHistogram("GenSimM800")->Scale(.017830);
  // getHistogram("GenSimM1300")->Scale(.003378);
  // getHistogram("GenSimM2000")->Scale(.001072);
  
  // getHistogram("RecoM300")->Scale(.656100);
  // getHistogram("RecoM800")->Scale(.017830);
  // getHistogram("RecoM1300")->Scale(.003378);
  // getHistogram("RecoM2000")->Scale(.001072);

  //CI 2016 Electron Weights

  // getHistogram("GenSimM300")->Scale(.578500);
  // getHistogram("GenSimM800")->Scale(.019000);
  // getHistogram("GenSimM1300")->Scale(.003954);
  // getHistogram("GenSimM2000")->Scale(.001052);
  
  // getHistogram("RecoM300")->Scale(.578500);
  // getHistogram("RecoM800")->Scale(.019000);
  // getHistogram("RecoM1300")->Scale(.003954);
  // getHistogram("RecoM2000")->Scale(.001052);

  //CI 2017 Electron Weights

  // getHistogram("GenSimM300")->Scale(.650200);
  // getHistogram("GenSimM800")->Scale(.017920);
  // getHistogram("GenSimM1300")->Scale(.003388);
  // getHistogram("GenSimM2000")->Scale(.001071);
  
  // getHistogram("RecoM300")->Scale(.650200);
  // getHistogram("RecoM800")->Scale(.017920);
  // getHistogram("RecoM1300")->Scale(.003388);
  // getHistogram("RecoM2000")->Scale(.001071);

  //ADD 2016 Weights: Lambda - 5000

  getHistogram("GenSimM1700")->Scale(.004171);
  getHistogram("RecoM1700")->Scale(.004171);

  //ADD 2017 Weights

  // getHistogram("GenSimM300")->Scale(1.90600000);
  // getHistogram("GenSimM800")->Scale(.04249000);
  // getHistogram("GenSimM1300")->Scale(.00531200);
  // getHistogram("GenSimM2000")->Scale(.00261800);
  
  // getHistogram("RecoM300")->Scale(1.90600000);
  // getHistogram("RecoM800")->Scale(.04249000);
  // getHistogram("RecoM1300")->Scale(.00531200);
  // getHistogram("RecoM2000")->Scale(.00261800);
  


  for (int i = 1; i < histBins; ++i)
    {
      for (auto massBin : massBins)
	{
	  getHistogram("GenSim Invariant Mass Pasted")->AddBinContent(i, getHistogram("GenSim" + massBin)->GetBinContent(i));
	  getHistogram("Reco Invariant Mass Pasted")->AddBinContent(i, getHistogram("Reco" + massBin)->GetBinContent(i));
	}
    }
}

bool PastingModule::isNewMassBin(const std::string mass)
{
  for (auto massBin : massBins)
    {
      if (mass == massBin)
	{
	  return false;
	}
    }
  
  return true;
}

