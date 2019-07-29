#ifndef ANALYSISMODULE_HH
#define ANALYSISMODULE_HH

#include <map>
#include <string>

#include "TH1.h"

#include "Module.hh"

class TH1;

class AnalysisModule : public Module
{
public:
  virtual void finalize();
  
protected:
  void addObject(const std::string& name, TObject* obj) {objects.insert({name, obj});}
  TObject* getObject(const std::string& name) {return objects.at(name);}
  const TObject* getObject(const std::string& name) const {return objects.at(name);}

  void makeHistogram(const std::string& name, const std::string& title, int nbins,
		     int min, int max);
  TH1* getHistogram(const std::string& name) {return dynamic_cast<TH1*>(getObject(name));}
  const TH1* getHistogram(const std::string& name) const
  {return dynamic_cast<const TH1*>(getObject(name));}
  void fillHistogram(const std::string& name, double number);

private:
  std::map<std::string, TObject*> objects;
};

#endif
