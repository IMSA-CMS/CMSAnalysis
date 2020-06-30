#include "CIAnalysis/CIStudies/interface/PastingModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TH1.h"

PastingModule::PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const PtResolutionModule& ptResModule, const LRWeightModule& lrWeightModule, int minMass, int maxMass, int minPt, int maxPt) :
  genSim(genSimModule),
  reco(recoModule),
  weighting(weightingModule),
  ptRes(ptResModule),
  lrWeighting(lrWeightModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  minPtCut(minPt),
  maxPtCut(maxPt)
{
}

void PastingModule::initialize()
{
  // for (HistogramPrototype* hist : histograms)
  // {
    // makeHistogram(hist);
  // }
  makeHistogram("GenSim Invariant Mass Pasted", "Gen Sim Invariant Mass Pasted", histBins, minMassCut, maxMassCut);
  makeHistogram("Reco Invariant Mass Pasted", "Reco Invariant Mass Pasted", histBins, minMassCut, maxMassCut);
  makeHistogram("GenSim Transverse Momentum Pasted", "Gen Sim Transverse Momentum Pasted", histBins, minPtCut, maxPtCut);
  makeHistogram("Reco Transverse Momentum Pasted", "Reco Transverse Momentum Pasted", histBins, minPtCut, maxPtCut);
}

bool PastingModule::process(const edm::EventBase& event)
{
  std::string massBin = getFileParams().getMassRange();
  std::string helicity = getFileParams().getHelicity();

  double eventWeight = 1.00;
  if (helicity == "LR")
    {
      eventWeight = lrWeighting.getLRWeight();
    }
  if (helicity == "RL")
    {
      eventWeight = lrWeighting.getRLWeight();
    }
  
  if (isNewMassBin(massBin))
    {
      auto weight = weighting.getWeight();
      auto fileKey = getFileParams().getFileKey();

      massBins[massBin] = weight;
      fileKeys[massBin] = fileKey;
      
      for (HistogramPrototype* hist : histograms)
	{
	  makeHistogram(hist->getName() + massBin, hist->getName() + massBin, hist->getNBins(), hist->getMinimum(), hist->getMaximum());
	}

      makeHistogram("Invariant Mass GenSim" + massBin, "Invariant Mass GenSim" + massBin, histBins, minMassCut, maxMassCut);
      makeHistogram("Invariant Mass Reco" + massBin, "Invariant Mass Reco" + massBin, histBins, minMassCut, maxMassCut);
      makeHistogram("Transverse Momentum GenSim" + massBin, "Transverse Momentum GenSim" + massBin, histBins, minPtCut, maxPtCut);
      makeHistogram("Transverse Momentum Reco" + massBin, "Transverse Momentum Reco" + massBin, histBins, minPtCut, maxPtCut);
    }

  auto genParticles = genSim.getGenParticles();
  auto genSimInv = genParticles.getInvariantMass();
  auto genSimPt = genParticles.getLeadingTransverseMomentum();


  auto recoParticles = reco.getRecoCandidates();
  auto recoInv = recoParticles.getInvariantMass();
  auto recoPt = recoParticles.getLeadingTransverseMomentum();
  
  edm::Handle<GenEventInfoProduct> genEvtInfo;
  event.getByLabel(std::string("generator"), genEvtInfo);
  //double qScale = genEvtInfo->pdf()->scalePDF;

  fillHistogram("Invariant Mass GenSim" + massBin, genSimInv, eventWeight);  
  fillHistogram("Invariant Mass Reco" + massBin, recoInv, eventWeight);
  fillHistogram("Transverse Momentum GenSim" + massBin, genSimPt, eventWeight);
  fillHistogram("Transverse Momentum Reco" + massBin, recoPt, eventWeight);
  // fillHistogram("Compare qScale", qScale);
  return true;
}

void PastingModule::finalize()
{
  for (auto massBin : massBins)
    {
      auto fileKey = fileKeys[massBin.first];
      auto eventCount = getEventCount(fileKey);

      std::cout << eventCount << std::endl;

      getHistogram("Invariant Mass GenSim" + massBin.first)->Scale(massBin.second / eventCount);
      getHistogram("Invariant Mass Reco" + massBin.first)->Scale(massBin.second / eventCount);
      getHistogram("Transverse Momentum GenSim" + massBin.first)->Scale(massBin.second / eventCount);
      getHistogram("Transverse Momentum Reco" + massBin.first)->Scale(massBin.second / eventCount);
    }


  for (int i = 1; i < histBins; ++i)
    {
      for (auto massBin : massBins)
	{
	  getHistogram("GenSim Invariant Mass Pasted")->AddBinContent(i, getHistogram("Invariant Mass GenSim" + massBin.first)->GetBinContent(i));
	  getHistogram("Reco Invariant Mass Pasted")->AddBinContent(i, getHistogram("Invariant Mass Reco" + massBin.first)->GetBinContent(i));
          getHistogram("GenSim Transverse Momentum Pasted")->AddBinContent(i, getHistogram("Transverse Momentum GenSim" + massBin.first)->GetBinContent(i));
          getHistogram("Reco Transverse Momentum Pasted")->AddBinContent(i, getHistogram("Transverse Momentum Reco" + massBin.first)->GetBinContent(i));
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



