#ifndef HISTOGRAMPROTOTYPE2D_HH
#define HISTOGRAMPROTOTYPE2D_HH

#include "HistogramPrototype.hh"

#include "TH2.h"

#include <string>

class HistogramPrototype2D : public HistogramPrototype
{
  public:
    HistogramPrototype2D(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY);
    int getNBinsX() const {return nBinsX;}
    int getNBinsY() const {return nBinsY;}
    double getMinX() const {return minX;}
    double getMinY() const {return minY;}
    double getMaxX() const {return maxX;}
    double getMaxY() const {return maxY;}

    std::vector<double> value() const override;  // Takes the vector of pairs from value2D() and makes it all into a single vector of doubles

    TH2* makeHistogram() const override;
    TH2* makeHistogram(std::string name, std::string title) const override;
  protected:
    virtual std::vector<std::pair<double, double>> value2D() const = 0;
  private:
    int nBinsX;
    int nBinsY;
    double minX;
    double minY;
    double maxX;
    double maxY;
};

#endif
