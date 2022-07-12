#ifndef IDTYPE_HH
#define IDTYPE_HH
#include <iostream>
#include <vector>

class IDType {
public:
  IDType(std::string name, std::vector<std::string> categories);
  
  // Getters
  std::string getName() const;
  std::vector<std::string> getCategories() const;

  // Checks if a category (passed in as a string) is in the vector of categories
  bool checkCategory(std::string category) const;

private:
  std::string name;
  std::vector<std::string> categories;
};
#endif