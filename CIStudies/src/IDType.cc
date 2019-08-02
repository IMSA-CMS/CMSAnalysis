#include "CIAnalysis/CIStudies/interface/IDType.hh"

#include <iostream>

std::string IDType::getValue() const
{
  return getMaps().names.at(value);
}

void IDType::setValue(const std::string& name)
{
  auto maps = getMaps();

  auto altIt = maps.alternates.find(name);
  if (altIt == maps.alternates.end())
    {
      throw std::runtime_error("Bad argument " + name + " passed to IDType::setValue()!");
    }

  for (unsigned index = 0; index < maps.names.size(); ++index)
    {
      if (maps.names[index] == altIt->second)
	{
	  value = index;
	  return;
	}
    }

  throw std::runtime_error("Bad argument " + name + " passed to IDType::setValue()!");
}

void IDType::addValueToMap(const std::string& value)
{
  // I assume the map has already been created
  // This will be true unless someone does something weird
  auto& localMaps = maps()[getTypeName()];

  localMaps.names.push_back(value);
  localMaps.alternates.insert({value, value});
}

void IDType::addAlternate(const std::string& alternate, const std::string& truth)
{
  auto& localMaps = maps()[getTypeName()];

  localMaps.alternates.insert({alternate, truth});
}

void IDType::addValuesToMap(const std::vector<std::string>& values)
{
  for (const auto& str : values)
    {
      addValueToMap(str);
    }
}
 
void IDType::addAlternates(const std::vector<std::string>& alternates, const std::string& truth)
{
  for (const auto& str : alternates)
    {
      addAlternate(str, truth);
    }
}

IDType::Maps& IDType::getMaps()
{
  static std::unordered_map<std::string, Maps>& staticMaps = maps();
  std::string type = getTypeName();

  auto it = staticMaps.find(type);
  if (it == staticMaps.end())
    {
      staticMaps.insert({type, {}});
      addMaps();
      it = staticMaps.find(type);
    }

  return it->second;
}

const IDType::Maps& IDType::getMaps() const
{
  return maps()[getTypeName()];
}

std::unordered_map<std::string, IDType::Maps>& IDType::maps()
{
  static std::unordered_map<std::string, Maps>* result = new std::unordered_map<std::string, Maps>();
  return *result;
}
