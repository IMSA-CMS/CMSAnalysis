#ifndef HISTOGRAMPROTOTYPE1DGENERAL_HH
#define HISTOGRAMPROTOTYPE1DGENERAL_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"

#include <functional>

#include <string>

class HistogramPrototype1DGeneral: public HistogramPrototype1D
{
  public:
    HistogramPrototype1DGeneral(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(const EventInput*)>function);
    virtual std::vector<double> value() const override {return vecOne(getInput());}
  private:
    std::function<std::vector<double>(const EventInput*)>vecOne;
    

};

#endif
