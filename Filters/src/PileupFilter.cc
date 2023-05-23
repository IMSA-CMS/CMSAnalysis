#include "CMSAnalysis/Filters/interface/PileupFilter.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include <iostream>

PileupFilter::PileupFilter(double loCut, double hiCut) :
  lowCutoff(loCut),
  highCutoff(hiCut)
{}

std::string PileupFilter::getFilterString(const InputModule* inputMod) const
{
  int interactions = inputMod ->getNumPileUpInteractions();

  if (interactions < lowCutoff)
  {
    return "LowPileup";
  }
  else if (interactions > highCutoff)
  {
    return "HighPileup";
  }
  return "";
}