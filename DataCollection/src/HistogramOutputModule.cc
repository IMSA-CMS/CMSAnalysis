#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"

//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/FileParams.hh"
#include "CMSAnalysis/DataCollection/interface/PtResolutionModule.hh"

#include <iostream>
#include <stdexcept>

#include "TH1.h"
#include "TH2.h"

void HistogramOutputModule::writeAll() {
  // Check if any baseObjects have not been copied to objects yet,
  // and fill them into the main map if not

  // Then write all the objects to file
  for (auto &entry : baseObjects) {
    entry.second->Write();
  }
}

void HistogramOutputModule::setInput(const InputModule *iInput) {
  Module::setInput(iInput);
  for (auto hist : histograms) {
    hist->setInput(iInput);
  }
}

void HistogramOutputModule::addObject(const std::string &name, TObject *obj) {
  if (baseObjects.find(name) == baseObjects.end()) {
    baseObjects.insert({name, obj});
  } else {
    baseObjects[name] = obj;
  }

  // std::cout << "Histogram added: " << name << '\n';
}

void HistogramOutputModule::makeHistogram(const std::string &name,
                                          const std::string &title, int nbins,
                                          double min, double max) {
  throw std::runtime_error("Don't use this function!");

  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void HistogramOutputModule::makeHistogram(
    std::shared_ptr<HistogramPrototype> h) {
  addObject(h->getFilteredName(), h->makeHistogram());
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h,
                                          std::string name) {
  addObject(name, h->makeHistogram(name, name));
}

void HistogramOutputModule::fillHistogram(const std::string &name,
                                          std::vector<double> values,
                                          double weight) {
  auto hist = getHistogram(name);
  if (!hist)
    throw std::runtime_error(
        "Argument to getHistogram was not of TH1 type!  Name: " + name +
        " and Root type: " + getObject(name)->ClassName());

  if (auto hist2D = dynamic_cast<TH2 *>(hist)) // If the hist is 2D hist
  {
    if (values.size() % 2 == 1) // If the size of values is odd
    {
      throw std::runtime_error(
          "Number of values in values vector is not even!");
    }

    for (int x = 0; x < static_cast<int>(values.size()); x += 2) {
      // Since x and y values are just inputed into the values vector as {x, y,
      // x, y, x, y, ...} The for loop loops through every other element in
      // values (i.e. every x value) The y value is then the element immediately
      // after the x element
      hist2D->Fill(values[x], values[x + 1], weight);
    }
  }

  else // If the hist is 1D hist
  {
    for (double currentNum : values) {
      hist->Fill(currentNum, weight);
    }
  }
}

std::string HistogramOutputModule::getObjectName(const std::string &str) const {
  std::string newName = getFilter() + str;
  return newName;
}

bool HistogramOutputModule::process() {
  for (auto hist : histograms) {
    bool draw = hist->shouldDraw(); // call the shouldDraw function so we can
                                    // call process on the FilterModules

    // If the histogram without mass bin doesn't exist, make it
    if (baseObjects.find(hist->getFilteredName()) == baseObjects.end()) {
      makeHistogram(hist, hist->getFilteredName());
    }

    // Fill the histogram if the filter string isn't empty
    if (hist->getName() !=
        hist->getFilteredName()) // If the filter string is empty, then the name
                                 // and the filtered name should be the same
    {
      makeHistogram(hist, hist->getFilteredName());
    }

    // Fill the histogram if shouldDraw(event) (draw) returns true
    if (draw) {
      fillHistogram(hist->getFilteredName(), hist->value());
    }
  }
  return true;
}
