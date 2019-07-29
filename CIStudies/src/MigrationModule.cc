#include "CIAnalysis/CIStudies/interface/MigrationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

MigrationModule::MigrationModule(const MatchingModule& matchingModule, int minMass, int maxMass, int massInterval) :
  matching(matchingModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}

void MigrationModule::initialize()
{
  const int numberOfBins = (maxMassCut - minMassCut) / interval;


  for(int i = 0; i < numberOfBins; ++i)
    {
      const std::string migrationStr = "Migration";
      makeHistogram(("genSimHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval, 
		    ((i - 2) * interval + minMassCut), ((i + 3) * interval + minMassCut));
      makeHistogram(("recoHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval,
		    ((i - 2) * interval + minMassCut), ((i + 3) * interval + minMassCut));
    }
}

bool MigrationModule::process(const edm::EventBase& event)
{
  auto bestPairs = matching.getMatchingBestPairs();

  if (bestPairs.getSize() >= 2)
    {
      double genSimMass = bestPairs.getGenParticles().getInvariantMass();
      double recoMass = bestPairs.getRecoParticles().getInvariantMass();

      std::string massBin = pickMassBin(genSimMass);
      fillHistogram("genSimHistBin" + massBin, genSimMass);
      fillHistogram("recoHistBin" + massBin, recoMass);
    }
  
  return true;
}

std::string MigrationModule::pickMassBin(double invariantMass)
{
  int mass = static_cast<int>(invariantMass) / interval * interval; //mass is invariantMass floored to the highest multiple of interval
  if (mass < minMassCut)
    {
      mass = minMassCut;
    }
  else if (mass > maxMassCut - interval)
    {
      mass = maxMassCut - interval;
    }

  return std::to_string(mass);
}

