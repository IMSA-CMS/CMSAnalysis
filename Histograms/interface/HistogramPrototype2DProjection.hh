#ifndef HISTOGRAMPROTOTYPE2DPROJECTION_HH
#define HISTOGRAMPROTOTYPE2DPROJECTION_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Utility/interface/HistogramPrototype2D.hh"

#include "TH1.h"

#include <string>

class HistogramPrototype2DProjection : public HistogramPrototype1D
{
  public:
    HistogramPrototype2DProjection(const std::string& iname, std::shared_ptr<HistogramPrototype2D> i2DHist, bool iXProjection,
       int iNBins = -1, double iminimum = -1, double imaximum = -1);
    virtual void setInput(const EventInput* iInput) override;

    std::vector<double> value() const override;

  private:
    std::shared_ptr<HistogramPrototype2D> hist2D;
	  bool xProjection;
};

#endif
