#ifndef IDTYPE_HH
#define IDTYPE_HH

#include <string>
#include <unordered_map>
#include <vector>

class IDType
{
public:
  std::string getValue() const;
  virtual std::string getTypeName() const = 0;
  void setValue(const std::string& name);

protected:
  void addValueToMap(const std::string& value);
  void addAlternate(const std::string& alternate, const std::string& truth);

  void addValuesToMap(const std::vector<std::string>& values);
  void addAlternates(const std::vector<std::string>& alternates, const std::string& truth);

  virtual void addMaps() = 0;

private:
  int value;

  struct Maps
  {
    std::vector<std::string> names;
    std::unordered_map<std::string, std::string> alternates;
  };

  // Slight inefficiency here, but it shouldn't be bad
  Maps& getMaps();
  const Maps& getMaps() const;
  
  // Uses the Construct Members on First Use idiom to avoid static initialization problem
  // Thanks to https://isocpp.org/wiki/faq/ctors#static-init-order
  static std::unordered_map<std::string, Maps>& maps();
};

#endif
