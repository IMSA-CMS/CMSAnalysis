#include "CMSAnalysis/DataCollection/interface/IDType.hh"
#include <vector>

// input "Mass 300 Lepton E Helicity 400" type is mass, lepton, helicity, value is 300, e, 400. 
std::string IDType::getName() const
{
  return name;
}

std::vector<std::string> IDType::getCategories() const
{
  return categories; 
}

bool IDType::checkCategory(std::string category) const
{
  for (const auto& category : categories) 
    {
      
        if (category == name)
        {
          return true;
        }
    }
  return false;
  // loop through vector of categories and see if it's equal. if it is, return true. if not at end, return false. 
}
