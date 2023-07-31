#include "CMSAnalysis/Filters/interface/CollinsSoperFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"

CollinsSoperFilter::CollinsSoperFilter(double hiCut) : // genSim(genSimModule),
                                                       highCutoff(hiCut){};

std::string CollinsSoperFilter::getFilterString(const EventInput* inputMod) const
{
  double collinsSoper = inputMod->getLeptons(EventInput::RecoLevel::GenSim).getCollinsSoper();

  if (abs(collinsSoper) > highCutoff)
    return "HighCS";
  else
    return "";
};
