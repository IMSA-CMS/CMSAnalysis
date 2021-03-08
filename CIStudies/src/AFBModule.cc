#include "CIAnalysis/CIStudies/interface/AFBModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

#include <iostream>

AFBModule::AFBModule(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule, int minMass, int maxMass, int massInterval) :
  HistogramOutputModule(genSimModule, recoModule, weightingModule, lrWeightModule),
  genSim(genSimModule),
  reco(recoModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}

void AFBModule::initialize()
{  
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
  makeHistogram("GenSimAFB", "GenSimAFB", histBins, minMassCut, maxMassCut);
  makeHistogram("RecoAFB", "RecoAFB", histBins, minMassCut, maxMassCut);

  for (int i = 1; i <= histBins; ++i)
    {
      auto genSimAFB = getAFB("GenSim", i);
      getHistogram("GenSimAFB")->SetBinContent(i, genSimAFB);

      auto recoAFB = getAFB("Reco", i);
      getHistogram("RecoAFB")->SetBinContent(i, recoAFB);      
    }

  AnalysisModule::finalize();
}

bool AFBModule::process(const edm::EventBase& event)
{
  auto genParticles = genSim->getGenParticles();
  auto genSimCS = genParticles.getCollinsSoper();
  auto genSimInvMass = genParticles.getInvariantMass();

  auto recoParticles = reco->getRecoCandidates();
  auto recoCS = recoParticles.getCollinsSoper();
  auto recoInvMass = recoParticles.getInvariantMass();

  fillFB("GenSim", genSimCS, genSimInvMass);
  fillFB("Reco", recoCS, recoInvMass);

  return true;
}

void AFBModule::fillFB(std::string level, double collinsSoper, double invariantMass)
{
  if (collinsSoper > 0)
    {
      fillHistogram(level + "Forward", invariantMass);
    }
  else if (collinsSoper == -2)
    {
      return;
    }
  else
    {
      fillHistogram(level + "Backward", invariantMass);
    }
}

double AFBModule::getAFB(std::string level, int binNumber)
{
  auto forward = getHistogram(level + "Forward")->GetBinContent(binNumber);
  auto backward = getHistogram(level + "Backward")->GetBinContent(binNumber);

  if (!(forward + backward == 0))
    {
      return (forward - backward) / (forward + backward);
    }
  else 
    {
      return 0;
    }
}
