#ifndef BJETFILTER_HH
#define BJETFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"


class BJetFilter : public Filter
{


protected:
  virtual std::string getFilterString(const InputModule* inputMod) const override;


  
};

#endif
