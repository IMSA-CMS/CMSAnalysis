#ifndef RESOLUTIONPROTOTYPE_HH
#define RESOLUTIONPROTOTYPE_HH

//#include "HistogramPrototype.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;
class ParticleCollection;

class ResolutionPrototype : public HistogramPrototype
{
  public:
    ResolutionPrototype(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
    double value() const override;
    virtual ~ResolutionPrototype() {} //empty virtual destructor

  protected:
    virtual double getValue(const ParticleCollection& partColl) const = 0; //pure virtual

  private:
    const GenSimIdentificationModule& genSim;
    const RecoIdentificationModule& reco;
};

#endif
