#include "CIAnalysis/CIStudies/interface/CollinsSoperFilter.hh"
//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

CollinsSoperFilter::CollinsSoperFilter(double hiCut) :
  //genSim(genSimModule),
  highCutoff(hiCut)
{}

std::string CollinsSoperFilter::makeFilterString()
{
  double collinsSoper = getInput()->getLeptons(InputModule::RecoLevel::GenSim).getCollinsSoper();

  if (abs(collinsSoper) > highCutoff)
    return "HighCS";
  else
    return "";
}
