#ifndef GENSIMRECOPROTOTYPE_HH
#define GENSIMRECOPROTOTYPE_HH

#include "HistogramPrototype.hh"

#include <string>

//class GenSimIdentificationModule;
//class RecoIdentificationModule;

class GenSimRecoPrototype : public HistogramPrototype
{
  public:
    GenSimRecoPrototype(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum);
    std::vector<double> value() const override {return {protectedValue(typeGenSim)};}     // Overloads value() and calls protectedValue(typeGenSim)
    virtual ~GenSimRecoPrototype() {} // Empty virtual destructor

  protected:
    //const std::shared_ptr<GenSimIdentificationModule> getGenSim() const {return genSim;}   // Getter for genSim
    //const std::shared_ptr<RecoIdentificationModule> getReco() const {return reco;}         // Getter for reco
    bool getDataType() const {return typeGenSim;}                          // Getter for typeGenSim
    virtual std::vector<double> protectedValue(bool typeSwitch) const = 0;              // Virtual function that is overridden in InvariantMassHist and PtHist
    
  private:
    bool typeGenSim;                            // Switch between GenSim and Reco: value is true for GenSim, false for Reco                       
};          

#endif
