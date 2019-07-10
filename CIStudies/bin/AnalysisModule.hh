#ifndef ANALYSISMODULE_HH
#define ANALYSISMODULE_HH

#include <unordered_map>
#include <string>

class TH1;
namespace edm
{
  class EventBase;
}

class AnalysisModule
{
public:
  // The destructor deletes the histograms, so don't destroy the object until the histograms are written
  virtual ~AnalysisModule();

  virtual void initialize();
  virtual void process(const edm::EventBase& event) = 0;
  virtual void command(const std::string&) {}

  void writeAllHistograms();
  
protected:
  virtual void createHistograms() = 0;
  void makeHistogram(const std::string& name, const std::string& title, int nbins, int min, int max);
  TH1* getHistogram(const std::string& name) {return histograms[name]};
  const TH1* getHistogram(const std::string& name) const {return histograms[name]};
  void fillHistogram(const std::string& name, double number) {histograms[name]->Fill(number);}

private:
  std::unordered_map<std::string, TH1*> histograms;
};

#endif
