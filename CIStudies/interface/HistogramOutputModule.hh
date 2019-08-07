#ifndef HISTOGRAMOUTPUTMODULE_HH
#define HISTOGRAMOUTPUTMODULE_HH

#include "AnalysisModule.hh"

class HistogramOutputModule : public AnalysisModule
{
public:
  virtual void writeAll();

protected:
  void addObject(const std::string& name, TObject* obj);
  TObject* getObject(const std::string& name) {return objects[getObjectName(name)];}
  const TObject* getObject(const std::string& name) const {return objects[getObjectName(name)];}

  void makeHistogram(const std::string& name, const std::string& title, int nbins,
		     double min, double max);
  TH1* getHistogram(const std::string& name) {return dynamic_cast<TH1*>(getObject(name));}
  const TH1* getHistogram(const std::string& name) const
  {return dynamic_cast<const TH1*>(getObject(name));}
  void fillHistogram(const std::string& name, double number);

private:
  std::map<std::string, TObject*> baseObjects;
  mutable std::map<std::string, TObject*> objects;

  std::string getObjectName(const std::string& str) const;
  void addObjectClone(const std::string& oldName, const std::string& newName) const;
};

#endif
