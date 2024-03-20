#ifndef HPLUSPLUSEFFICIENCY_HH
#define HPLUSPLUSEFFICIENCY_HH

#include "EfficiencyModule.hh"
#include <map>

//Calculates reconstruction efficiency for H++
class HPlusPlusEfficiency : public EfficiencyModule
{
public:
  HPlusPlusEfficiency();

  void doCounters() override;
  void finalize() override;
  
private:

  const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
  const std::vector<double> genSimDecayCounts{2200.000000, 8646.000000, 8740.000000, 4417.000000, 8668.000000, 4298.000000, 8600.000000, 17415.000000, 8762.000000, 17369.000000, 8693.000000, 8719.000000, 8707.000000, 17340.000000, 8672.000000, 2209.000000, 8722.000000, 4329.000000, 8687.000000, 8596.000000, 2210.000000};
  const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
  const double numEvents = 832249.000000;
  
};

#endif