#include "CMSAnalysis/DataCollection/interface/CollinsSoperFilter.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"

CollinsSoperFilter::CollinsSoperFilter(double hiCut) : // genSim(genSimModule),
                                                       highCutoff(hiCut){};

std::string CollinsSoperFilter::makeFilterString()
{
  double collinsSoper = getInput()->getLeptons(InputModule::RecoLevel::GenSim).getCollinsSoper();

  if (abs(collinsSoper) > highCutoff)
    return "HighCS";
  else
    return "";
};
