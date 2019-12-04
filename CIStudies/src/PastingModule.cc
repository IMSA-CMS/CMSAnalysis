#include "CIAnalysis/CIStudies/interface/PastingModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TH1.h"

PastingModule::PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, int minMass, int maxMass) :
  genSim(genSimModule),
  reco(recoModule),
  weighting(weightingModule),
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
      auto weight = weighting.getWeight();
      auto fileKey = getFileParams().getFileKey();

      massBins[massBin] = weight;
      fileKeys[massBin] = fileKey;

      makeHistogram("GenSim" + massBin, "GenSim" + massBin, histBins, minMassCut, maxMassCut);
      makeHistogram("Reco" + massBin, "Reco" + massBin, histBins, minMassCut, maxMassCut);;
    }

  auto genParticles = genSim.getGenParticles();
  auto genSimInv = genParticles.getInvariantMass();

  auto recoParticles = reco.getRecoCandidates();
  auto recoInv = recoParticles.getInvariantMass();
  
  edm::Handle<GenEventInfoProduct> genEvtInfo;
  event.getByLabel(std::string("generator"), genEvtInfo);
  double qScale = genEvtInfo->pdf()->scalePDF;

  fillHistogram("GenSim" + massBin, genSimInv);  
  fillHistogram("Reco" + massBin, recoInv);
  fillHistogram("Compare qScale", qScale);
  return true;
}

void PastingModule::finalize()
{
  for (auto massBin : massBins)
    {
      auto fileKey = fileKeys[massBin.first];
      auto eventCount = getEventCount(fileKey);

      std::cout << eventCount << std::endl;

      getHistogram("GenSim" + massBin.first)->Scale(massBin.second / eventCount);
      getHistogram("Reco" + massBin.first)->Scale(massBin.second / eventCount) ;
    }


  for (int i = 1; i < histBins; ++i)
    {
      for (auto massBin : massBins)
	{
	  getHistogram("GenSim Invariant Mass Pasted")->AddBinContent(i, getHistogram("GenSim" + massBin.first)->GetBinContent(i));
	  getHistogram("Reco Invariant Mass Pasted")->AddBinContent(i, getHistogram("Reco" + massBin.first)->GetBinContent(i));
	}
    }
}

bool PastingModule::isNewMassBin(const std::string mass)
{
  for (auto massBin : massBins)
    {
      if (mass == massBin.first)
	{
	  return false;
	}
    }
  
  return true;
}

