#ifndef HISTOGRAMOUTPUTMODULE_HH
#define HISTOGRAMOUTPUTMODULE_HH

#include <map>
#include <string>

#include "AnalysisModule.hh"
#include "TH1.h"

class TObject;
class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class PtResolutionModule;
class HistogramPrototype;

// An AnalysisModule designed to fill histograms into a Root file
class HistogramOutputModule : public AnalysisModule
{
public:
  HistogramOutputModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule);
  virtual void writeAll();
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;
  void addHistogram(HistogramPrototype* hist) {histograms.push_back(hist);}; // Adds a HistogramPrototype* to histogram (the vector)

protected:
  // This adds an object to the collection to be written.
  // All objects that use this method are duplicated for different filter categories
  // and written to a Root file by writeAll().
  // If you add two objects of the same name, the second will replace the first.
  void addObject(const std::string& name, TObject* obj);

  // Gets objects from the collection.
  // These automatically get the version for the current filter category.
  // You do NOT need to consider filter names in your code - the implementation
  // takes care of that.  Simply code as though there was only one set of histograms,
  // and they will duplicate automatically as needed.
  TObject* getObject(const std::string& name) {return objects[getObjectName(name)];}
  const TObject* getObject(const std::string& name) const 
  {return objects[getObjectName(name)];}

  // Creates a histogram and adds it to the collection.  Just a convenient shortcut
  // for addObject() above.
  void makeHistogram(const std::string& name, const std::string& title, int nbins,
		     double min, double max);

  // Creates a histogram from a HistogramPrototype* and adds it to the collection.
  void makeHistogram(HistogramPrototype* h);

  // Convenient getter methods to access histograms
  TH1* getHistogram(const std::string& name) {return dynamic_cast<TH1*>(getObject(name));}
  const TH1* getHistogram(const std::string& name) const
  {return dynamic_cast<const TH1*>(getObject(name));}

  // Convenient function to fill a histogram by name
  void fillHistogram(const std::string& name, double number);

private:
  // This is a map of objects as they are seen by the user, by name
  std::map<std::string, TObject*> baseObjects;

  // These are the true, underlying objects, with a clone made for each
  // different filter string.
  mutable std::map<std::string, TObject*> objects;

  // This calculates the true object name based on the filter string
  std::string getObjectName(const std::string& str) const;

  // This clones from the baseObjects map into the objects map, which must
  // be done before anyone can work profitably with the object
  void addObjectClone(const std::string& oldName, const std::string& newName) const;

  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  const WeightingModule& weighting;

  std::unordered_map<std::string, double> massBins;
  std::unordered_map<std::string, std::string> fileKeys;

  bool isNewMassBin(const std::string mass);
  std::vector<HistogramPrototype*> histograms;
};

#endif
