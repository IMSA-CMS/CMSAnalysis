// #include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"
// #include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
// #include <string>

// #include "TH1.h"

// void HistogramOutputModule::makeHistogram(HistogramPrototype* h)
// {
  // const std::string& name = h->getName();
  // auto title = name;
  // int nbins = h->getNBins();
  // double min = h->getMinimum();
  // double max = h->getMaximum();
  
  // auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  // addObject(name, newHist);
// }


