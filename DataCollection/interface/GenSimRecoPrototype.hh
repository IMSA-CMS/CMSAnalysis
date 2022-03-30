#ifndef GENSIMRECOPROTOTYPE_HH
#define GENSIMRECOPROTOTYPE_HH

#include "HistogramPrototype1D.hh"
#include "InputModule.hh"

#include <string>

class GenSimRecoPrototype : public HistogramPrototype1D
{
  public:
    GenSimRecoPrototype(InputModule::RecoLevel type, const std::string& iname, int iNBins, double iminimum, double imaximum);
    std::vector<double> value() const override {return {protectedValue(typeGenSim)};}     // Overloads value() and calls protectedValue(typeGenSim)
    virtual ~GenSimRecoPrototype() {} // Empty virtual destructor

  protected:
    //const std::shared_ptr<GenSimIdentificationModule> getGenSim() const {return genSim;}   // Getter for genSim
    //const std::shared_ptr<RecoIdentificationModule> getReco() const {return reco;}         // Getter for reco
    bool getDataType() const {return typeGenSim;}                          // Getter for typeGenSim
  virtual std::vector<double> protectedValue(InputModule::RecoLevel level) const = 0;              // Virtual function that is overridden in InvariantMassHist and PtHist
    
  private:
  InputModule::RecoLevel typeGenSim;                         // Switch between GenSim and Reco: value is true for GenSim, false for Reco                       
};          

#endif
