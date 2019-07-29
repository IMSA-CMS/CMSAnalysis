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

bool AcceptanceModule::process(const edm::EventBase& event)
{
  double genSimParticle = genSim.getGenParticles().getInvariantMass();
  fillHistogram("GenSimParticles", genSimParticle);
  
  if (matching.getMatchingBestPairs().getSize() >= 2)
    {
      fillHistogram("Acceptance", genSimParticle);
      fillHistogram("RecoParticles", genSimParticle);
    }
  
  return true;
}

void AcceptanceModule::writeAllHistograms()
{
  getHistogram("Acceptance")->Divide(getHistogram("GenSimParticles"));
  
  AnalysisModule::writeAllHistograms();
}

void AcceptanceModule::createHistograms()
{
  makeHistogram("Acceptance", "Acceptance", interval, minMassCut, maxMassCut);
  makeHistogram("GenSimParticles", "GenSimParticles", interval, minMassCut, maxMassCut);
  makeHistogram("RecoParticles", "RecoParticles", interval, minMassCut, maxMassCut);
}
