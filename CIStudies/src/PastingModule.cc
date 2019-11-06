#include "CIAnalysis/CIStudies/interface/PastingModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include "TH1.h"

PastingModule::PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, int minMass, int maxMass, int massInterval) :
  genSim(genSimModule),
  reco(recoModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
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
  for (auto massBin : massBins)
    {
      getHistogram("GenSim" + massBin)->Scale(getHistogram("GenSim" + massBin)->Integral() / getHistogram("GenSim" + massBins[0])->Integral());
      getHistogram("Reco" + massBin)->Scale(getHistogram("Reco" + massBin)->Integral() / getHistogram("GenSim" + massBins[0])->Integral());
    }

//"CITo2Mu_Lam16TeVConL
  //getHistogram("GenSimM300")->Scale(.579800); ,
  //getHistogram("GenSimM800")->Scale(.019020);
  //getHistogram("GenSimM1300")->Scale(.003909);
  //getHistogram("GenSimM2000")->Scale(.001052);
  
  //getHistogram("RecoM300")->Scale(.578500);
  //getHistogram("RecoM800")->Scale(.019000);
  //getHistogram("RecoM1300")->Scale(.003954);
  //getHistogram("RecoM2000")->Scale(.001052);

  //ADD 2016
  //getHistogram("GenSimM1700")->Scale(.004171);
  //getHistogram("RecoM1700")->Scale(.004171);

  //ADD 2017
  getHistogram("GenSimM300")->Scale(1.90600000);
  getHistogram("GenSimM800")->Scale(.042490000);
  getHistogram("GenSimM1300")->Scale(.00531200);
  getHistogram("GenSimM2000")->Scale(.00261800);
 
  getHistogram("RecoM300")->Scale(1.90600000);
  getHistogram("RecoM800")->Scale(.042490000);
  getHistogram("RecoM1300")->Scale(.00531200);
  getHistogram("RecoM2000")->Scale(.00261800);



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

