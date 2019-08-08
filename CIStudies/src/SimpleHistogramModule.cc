#include "CIAnalysis/CIStudies/interface/SimpleHistogramModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

#include <iostream>

SimpleHistogramModule::SimpleHistogramModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule) :
  genSim(genSimModule),
  reco(recoModule)
{
}

void SimpleHistogramModule::initialize()
{  
  const int cosMin = -1;
  const int cosMax = 1;

  makeHistogram("GenSimCS", "GenSimCS", histBins, cosMin, cosMax);
  makeHistogram("RecoCS", "RecoCS", histBins, cosMin, cosMax);
}

bool SimpleHistogramModule::process(const edm::EventBase& event)
{
  auto genParticles = genSim.getGenParticles();
  auto genSimCS = genParticles.getCollinsSoper();

  auto recoParticles = reco.getRecoCandidates();
  auto recoCS = recoParticles.getCollinsSoper();

  fillHistogram("GenSimCS", genSimCS);
  fillHistogram("RecoCS", recoCS);

  return true;
}