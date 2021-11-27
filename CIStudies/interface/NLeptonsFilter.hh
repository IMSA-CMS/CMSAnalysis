#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

//class RecoIdentificationModule;

class NLeptonsFilter : public FilterModule
{
  public:
  NLeptonsFilter(double pTCut = 0);
  protected:
    virtual std::string makeFilterString() override;
  private:
    double pTMin; 
    // const std::shared_ptr<RecoIdentificationModule> recoModule;
};

#endif
