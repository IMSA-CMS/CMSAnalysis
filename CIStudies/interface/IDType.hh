#ifndef IDTYPE_HH
#define IDTYPE_HH

#include <string>
#include <unordered_map>
#include <vector>

// A class to hold information about a distinguishing type, like a dynamic enum.
// Used mainly for different helicity models, interference models, mass cuts, etc.
// Everything is implemented in a static map, so all values must be global for the whole program.
// This improves efficiency.
class IDType
{
public:
  virtual ~IDType() {};
  // Gets and sets the current value of the type
  // e.g. "Electron"
  std::string getValue() const;
  void setValue(const std::string& name);

  // This is the type name of the particular instance - again, just like an enum
  // e.g. "LeptonType"
  virtual std::string getTypeName() const = 0;

protected:
  // Adds a new value to the static map (basically a new enum value)
  void addValueToMap(const std::string& value);
  // Adds an alternate version of the value, to allow more robust input
  void addAlternate(const std::string& alternate, const std::string& truth);

  // Multiple-value versions of the above, for convenience
  void addValuesToMap(const std::vector<std::string>& values);
  void addAlternates(const std::vector<std::string>& alternates, const std::string& truth);

  // This is called once per subclass for the lifetime of the program.
  // It needs to initialize all the maps, as it is guaranteed to be called
  // before anyone uses the value.
  virtual void addMaps() = 0;

private:
  // Note that the value is stored as an int, but the int is not accessible.
  // The user only sees the string that the int maps to.
  int value;

  // A simple struct of possible names, and alternates of those names, for a given object
  // For example, a LeptonType would have names Electron and Muon, and you can map 
  // Ele, el, e, E, and El to Electron, and similar alternates for Muon.
  // The alternates map automatically contains the original name for implementatino convenience,
  // so that does not need to added separately.
  struct Maps
  {
    std::vector<std::string> names;
    std::unordered_map<std::string, std::string> alternates;
  };

  // Gets the relevant Maps object for this type.
  // If they have not yet been filled, it calls addMaps().
  Maps& getMaps();
  const Maps& getMaps() const;

  // The place where the actual maps are all stored statically.
  // Uses the Construct Members on First Use idiom to avoid static initialization problem.
  // Thanks to https://isocpp.org/wiki/faq/ctors#static-init-order
  static std::unordered_map<std::string, Maps>& maps();
};

#endif
