#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

MassFilter::MassFilter(const GenSimIdentificationModule& genSimModule, double hiCut) :
  genSim(genSimModule),
  highCutoff(hiCut)
{}

std::string MassFilter::makeFilterString(const edm::EventBase& event)
{
  // double invariantMass = genSim.getGenParticles().getInvariantMass();

  // if (invariantMass > highCutoff)
  //   return "HighMass";
  // else
  //   return "";

  std::string massBin = getFileParams().getMassRange();
  int massBinNumber = std::stoi(massBin.substr(1));

  int massBinUpperCutoff = 0;

  if (massBinNumber == 300)
    massBinUpperCutoff = 800;
  else if (massBinNumber == 800)
    massBinUpperCutoff = 1300;
  else if (massBinNumber == 1300)
    massBinUpperCutoff = 2000;
  else if (massBinNumber == 2000)
    massBinUpperCutoff = 5000;

  edm::Handle<GenEventInfoProduct> genEvtInfo;
  event.getByLabel(std::string("generator"), genEvtInfo);
  double qScale = genEvtInfo->pdf()->scalePDF;
  
  if (qScale <= massBinUpperCutoff)
     return "Included";
  else
    return "";
}
