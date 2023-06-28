#ifndef GENSIMRECOPROTOTYPE_HH
#define GENSIMRECOPROTOTYPE_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

#include <string>

class GenSimRecoPrototype : public HistogramPrototype1D
{
  public:
    GenSimRecoPrototype(EventInput::RecoLevel type, const std::string& iname, int iNBins, double iminimum, double imaximum);
    std::vector<double> value() const override {return {protectedValue(typeGenSim)};}     // Overloads value() and calls protectedValue(typeGenSim)
    virtual ~GenSimRecoPrototype() {} // Empty virtual destructor

  protected:
    EventInput::RecoLevel getDataType() const {return typeGenSim;}  // typeGenSim getter
  virtual std::vector<double> protectedValue(EventInput::RecoLevel level) const = 0;  // Virtual function that is overridden in InvariantMassHist and PtHist
    
  private:
  EventInput::RecoLevel typeGenSim;    // Switch between GenSim and Reco: true for GenSim, false for Reco                       
};          

#endif
