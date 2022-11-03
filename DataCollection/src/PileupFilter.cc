#include "CMSAnalysis/DataCollection/interface/PileupFilter.hh"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include <iostream>

PileupFilter::PileupFilter(double loCut, double hiCut) :
  lowCutoff(loCut),
  highCutoff(hiCut)
{}

std::string PileupFilter::makeFilterString()
{
  int interactions = getInput()->getNumPileUpInteractions();

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