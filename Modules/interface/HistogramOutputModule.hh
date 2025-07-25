#ifndef HISTOGRAMOUTPUTMODULE_HH
#define HISTOGRAMOUTPUTMODULE_HH

#include <map>
#include <string>

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

#include "TH1.h"
#include "TH2.h"

class TObject;
class PtResolutionModule;
class HistogramPrototype;
class EventInput;

// An AnalysisModule designed to fill histograms into a Root file
class HistogramOutputModule : public AnalysisModule
{
public:
  virtual void finalizeFilterString(TFile *outFile) override;
  virtual void initialize() override {};     // Empty function
  virtual void finalize() override;
  virtual bool process() override;    // Fills the histograms
  void addHistogram(std::shared_ptr<HistogramPrototype> hist); // Adds a HistogramPrototype* to histogram (the vector)
  virtual void setInput(const EventInput* iInput) override;
  void addScaleFactor(std::shared_ptr<ScaleFactor> scaleFactor);
  virtual std::string getName() override {return "HistogramOutputModule";}
protected:
  // This adds an object to the collection to be written.
  // All objects that use this method are duplicated for different filter categories
  // If you add two objects of the same name, the second will replace the first.
  void addObject(std::string name, std::string path, TObject* obj);

  // Gets objects from the collection.
  // These automatically get the version for the current filter category.
  // You do NOT need to consider filter names in your code - the implementation
  // takes care of that.  Simply code as though there was only one set of histograms,
  // and they will duplicate automatically as needed.
  TObject* getObject(const std::string& name); 
  const TObject* getObject(const std::string& name) const; 

  // Creates a histogram and adds it to the collection.  Just a convenient shortcut
  // for addObject() above.
  void makeHistogram(const std::string& name, const std::string& title, int nbins,
		     double min, double max);
  // Creates a histogram and adds it to the collection, but uses the HistogramPrototype* object
  // void makeHistogram(HistogramPrototype* h);

  // Creates a histogram from a HistogramPrototype* and adds it to the collection.
  void makeHistogram(std::shared_ptr<HistogramPrototype> h, std::string name);

  // Convenient getter methods to access histograms
  TH1* getHistogram(const std::string& name) {return dynamic_cast<TH1*>(getObject(name));}
  const TH1* getHistogram(const std::string& name) const
  {return dynamic_cast<const TH1*>(getObject(name));}

  // Convenient function to fill a histogram by name
  void fillHistogram(const std::string& name, std::vector<double> vectors, double weight = 1.00);

private:
  std::string getObjectName(const std::string &str) const;
  // This is a map of objects as they are seen by the user, by name
  std::map<std::string, std::tuple<std::string, TObject *>> baseObjects;
  std::vector<std::shared_ptr<ScaleFactor>> scaleFactors;

  std::vector<std::shared_ptr<HistogramPrototype>> histograms; 
  

  float total = 0;
};

#endif
