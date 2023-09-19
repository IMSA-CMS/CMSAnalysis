#ifndef HPLUSPLUSEFFICIENCY_HH
#define HPLUSPLUSEFFICIENCY_HH

#include "EfficiencyModule.hh"

//Calculates reconstruction efficiency for H++
class HPlusPlusEfficiency : public EfficiencyModule
{
public:
  HPlusPlusEfficiency();

  void doCounters() override;
  void finalize() override;
  
private:

  const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
  const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
  std::map<std::string, int> numbers;
};

#endif