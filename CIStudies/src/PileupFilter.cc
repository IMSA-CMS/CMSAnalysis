#include "CIAnalysis/CIStudies/interface/PileupFilter.hh"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include <iostream>

PileupFilter::PileupFilter(double loCut, double hiCut) :
  lowCutoff(loCut),
  highCutoff(hiCut)
{}

std::string PileupFilter::makeFilterString(const edm::EventBase& event)
{
  edm::Handle<std::vector<PileupSummaryInfo>> pileup;
  event.getByLabel(std::string("slimmedAddPileupInfo"), pileup);

  int interactions = (*pileup)[0].getPU_NumInteractions();

  if (interactions < lowCutoff)
    return "LowPileup";
  else if (interactions > highCutoff)
    return "HighPileup";
  else
    return "";
}
