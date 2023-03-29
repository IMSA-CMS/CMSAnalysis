#ifndef RESOLUTIONPROTOTYPE_HH
#define RESOLUTIONPROTOTYPE_HH

#include <memory>

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;
class Lepton;

class ResolutionPrototype : public HistogramPrototype1D
{
  public:
    ResolutionPrototype(const std::string& iname, int iNBins, double iminimum, double imaximum);
    std::vector<double> value() const override;
    virtual ~ResolutionPrototype() {} //empty virtual destructor

  protected:
    virtual double getValue(const ParticleCollection<Lepton>& partColl) const = 0; //pure virtual
};

#endif
