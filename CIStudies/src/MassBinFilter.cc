#include "CIAnalysis/CIStudies/interface/MassBinFilter.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

MassBinFilter::MassBinFilter(const std::shared_ptr<MatchingModule> matchingModule, double iminimum, double imaximum, int iNBins) :
  matching(matchingModule),
  minimum(iminimum),
  maximum(imaximum),
  nBins(iNBins)
{
}

std::string MassBinFilter::makeFilterString(const edm::EventBase& event)
{
  auto bestPairs = matching->getMatchingBestPairs();   // Get the matching pairs

  if (bestPairs.getSize() < 2)  // If the pairs don't match
    {
      return "";
    }

  double genSimMass = bestPairs.getGenParticles().getInvariantMass();  // Find the invariant mass of the pair

  double interval = (maximum - minimum) / nBins;

  int mass = static_cast<int>(genSimMass / interval) * interval;    // mass is genSimMass floored to the highest multiple of interval
  if (mass < minimum)
    {
      mass = minimum;
    }
  else if (mass > maximum - interval)
    {
      mass = maximum - interval;
    }

  std::string low = std::to_string(mass);
  std::string high = std::to_string(static_cast<int>(mass + interval));
  std::string massBin = low + "-" + high;

  return massBin;
}
