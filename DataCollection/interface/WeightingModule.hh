#ifndef WEIGHTINGMODULE_HH
#define WEIGHTINGMODULE_HH

#include <vector>
#include "ProductionModule.hh"
#include <unordered_map>

class WeightingModule : public ProductionModule
{
public:
  WeightingModule();
  virtual bool process() override;
  double getWeight() const {return weight;}

private:
  void findWeight(std::string key);
  std::unordered_map<std::string, double> weights;
  double weight;
};

#endif
