#ifndef RESOLUTIONPROTOTYPE_HH
#define RESOLUTIONPROTOTYPE_HH

#include <memory>

#include "CIAnalysis/CIStudies/interface/HistogramPrototype1D.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;
class ParticleCollection;

class ResolutionPrototype : public HistogramPrototype1D
{
  public:
    ResolutionPrototype(const std::string& iname, int iNBins, double iminimum, double imaximum);
    std::vector<double> value() const override;
    virtual ~ResolutionPrototype() {} //empty virtual destructor

  protected:
    virtual double getValue(const ParticleCollection& partColl) const = 0; //pure virtual
};

#endif
