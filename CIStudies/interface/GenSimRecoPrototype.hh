#ifndef GENSIMRECOPROTOTYPE_HH
#define GENSIMRECOPROTOTYPE_HH

#include "HistogramPrototype.hh"

#include <string>

class GenSimIdentificationModule;
class RecoIdentificationModule;


class GenSimRecoPrototype : public HistogramPrototype
{
  public:
    GenSimRecoPrototype(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const bool typeSwitch, std::string& iname, int iNBins, double iminimum, double imaximum);
    double value() const override {return protectedValue(typeGenSim);}     // Overloads value() and calls protectedValue(typeGenSim)
    virtual ~GenSimRecoPrototype() {} // Empty virtual destructor

    virtual double protectedValue(bool typeSwitch) const = 0;
  private:
    const GenSimIdentificationModule& genSim; 
    const RecoIdentificationModule& reco;
    bool typeGenSim;                            // Switch between GenSim and Reco: value is true for GenSim, false for Reco                       
};          


#endif
