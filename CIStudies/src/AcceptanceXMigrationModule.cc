#include "CIAnalysis/CIStudies/interface/AcceptanceXMigrationModule.hh"
#include "CIAnalysis/CIStudies/interface/CommonValuesModule.hh"

AcceptanceXMigrationModule::AcceptanceXMigrationModule(const CommonValuesModule& commonModule, int minMass, int maxMass, int massInterval) :
  common(commonModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}


bool AcceptanceXMigrationModule::process(const edm::EventBase& event)
{
  std::string massBin = pickMassBin(common.getInvariantMass().genSim);
  fillHistogram("genSimHistBin" + massBin, common.getInvariantMass().genSim);
  fillHistogram("recoHistBin" + massBin, common.getInvariantMass().reco);
  
  return true;
}

void AcceptanceXMigrationModule::createHistograms()
{
  const int numberOfBins = (maxMassCut - minMassCut) / interval;


  for(int i = 0; i < numberOfBins; ++i)
    {
      const std::string migrationStr = "Migration";
      makeHistogram(("genSimHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval, (i * 100 + minMassCut - 200), (i * 100 + minMassCut + 300));
      makeHistogram(("recoHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval, (i * 100 + minMassCut - 200), (i * 100 + minMassCut + 300));
    }
}

std::string AcceptanceXMigrationModule::pickMassBin(double invariantMass)
{
  int mass = static_cast<int>(invariantMass) / interval * interval; //mass is invariantMass floored to the highest multiple of interval
  if (mass < minMassCut)
    {
      mass = minMassCut;
    }
  else if (mass > maxMassCut)
    {
      mass = maxMassCut;
    }

  return std::to_string(mass);
}

