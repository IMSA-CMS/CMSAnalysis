#ifndef IDTYPE_HH
#define IDTYPE_HH
#include <iostream>
#include <vector>

class IDType {
public:
  IDType(std::string name, std::vector<std::string> categories);
  std::string getName() const;
  std::vector<std::string> getCategories() const;
  bool checkCategory(std::string category) const;

private:
  std::string name;
  std::vector<std::string> categories;
};
#endif