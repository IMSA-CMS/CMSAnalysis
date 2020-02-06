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
  const int invMin = 0;
  const int invMax = 3300;

  makeHistogram("GenSimCS", "GenSimCS", histBins, cosMin, cosMax);
  makeHistogram("RecoCS", "RecoCS", histBins, cosMin, cosMax);
  makeHistogram("GenSimInv", "GenSimInv", histBins, invMin, invMax);
  makeHistogram("RecoInv", "RecoInv", histBins, invMin, invMax);
}

bool SimpleHistogramModule::process(const edm::EventBase& event)
{
  auto genParticles = genSim.getGenParticles();
  auto genSimCS = genParticles.getCollinsSoper();

  auto recoParticles = reco.getRecoCandidates();
  auto recoCS = recoParticles.getCollinsSoper();

  auto genSimInv  = genParticles.getInvariantMass();
  auto recoInv = recoParticles.getInvariantMass();


  if (genSimCS != -2)
    {
      fillHistogram("GenSimCS", genSimCS);
    }

  if (recoCS != -2)
    {
      fillHistogram("RecoCS", recoCS);
    }
  
  // if (genSimCS != -2 && abs(recoCS) < .975)
  //   {     
  //     std::cout << "Gen Sim Leptons: " << genParticles.getNumParticles() << "      Reco Leptons: " << recoParticles.getNumParticles() << std::endl;
  //   }

  fillHistogram("GenSimInv", genSimInv);
  fillHistogram("RecoInv", recoInv);

  return true;
}
