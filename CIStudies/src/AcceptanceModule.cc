#include "CIAnalysis/CIStudies/interface/AcceptanceModule.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

AcceptanceModule::AcceptanceModule(const GenSimIdentificationModule& genSimModule, const MatchingModule& matchingModule, int minMass, int maxMass, int massInterval) :
  genSim(genSimModule),
  matching(matchingModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}


void AcceptanceModule::initialize()
{
  Module::initialize();

  makeHistogram("GenSimParticles", "GenSimParticles", interval, minMassCut, maxMassCut);
  makeHistogram("RecoParticles", "RecoParticles", interval, minMassCut, maxMassCut);
}

bool AcceptanceModule::process(const edm::EventBase& event)
{
  double genSimParticle = genSim.getGenParticles().getInvariantMass();
  fillHistogram("GenSimParticles", genSimParticle);
  
  if (matching.getMatchingBestPairs().getSize() >= 2)
    {
      fillHistogram("RecoParticles", genSimParticle);
    }
  
  return true;
}

void AcceptanceModule::finalize()
{
  auto acceptanceHist = dynamic_cast<TH1*>(getHistogram("RecoParticles")->Clone());
  acceptanceHist->Divide(getHistogram("GenSimParticles"));
  addObject("Acceptance", acceptanceHist);
  
  AnalysisModule::finalize();
}
