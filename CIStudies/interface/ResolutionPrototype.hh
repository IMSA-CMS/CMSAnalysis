#ifndef RESOLUTIONPROTOTYPE_HH
#define RESOLUTIONPROTOTYPE_HH

#include "HistogramPrototype.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;

class ResolutionPrototype : public HistogramPrototype
{
  public:
    ResolutionPrototype(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
    double value() const override {return ((recoMass - genSimMass)/genSimMass);}
    virtual ~ResolutionPrototype() {} //empty virtual destructor

  private:
    const double genSimMass;
    const double recoMass;
};

#endif
