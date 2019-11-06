#ifndef WEIGHTINGMODULE_HH
#define WEIGHTINGMODULE_HH

#include <vector>
#include "ProductionModule.hh"

class WeightingModule : public ProductionModule
{
public:
  virtual bool process(const edm::EventBase& event) override;
  double getWeight() {return weight;}

private:
  void findWeight(std::vector<std::string> values);
  double weight;
};

#endif
