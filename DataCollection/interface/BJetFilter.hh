#ifndef BJETFILTER_HH
#define BJETFILTER_HH

#include <memory>
#include "FilterModule.hh"


class BJetFilter : public FilterModule
{


protected:
  virtual std::string makeFilterString() override;


  
};

#endif
