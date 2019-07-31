#include "CIAnalysis/CIStudies/interface/AnalysisModule.hh"

#include <iostream>
#include <stdexcept>

#include "TH1.h"

void AnalysisModule::writeAll()
{
  // Check if any baseObjects have not been copied to objects yet
  for (auto entry : baseObjects)
    {
      bool foundIt = false;
      for (auto entry2 : objects)
	{
	  if (entry2.first.find(entry.first) != std::string::npos)
	    {
	      foundIt = true;
	      break;
	    }
	}

      if (!foundIt)
	{
	  addObjectClone(entry.first, entry.first);
	}
    }

  for (auto& entry : objects)
    {
      entry.second->Write();
    }
}

void AnalysisModule::addObject(const std::string& name, TObject* obj)
{
  if (baseObjects.find(name) == baseObjects.end())
    baseObjects.insert({name, obj});
  else
    baseObjects[name] = obj;

  if (isFinalStep)
    addObjectClone(name, currentFilter + name);
}

void AnalysisModule::makeHistogram(const std::string& name, const std::string& title, int nbins, int min, int max)
{
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void AnalysisModule::fillHistogram(const std::string& name, double number)
{
  auto hist = getHistogram(name);
  if (!hist)
    throw std::runtime_error("Argument to getHistogram was not of TH1 type!  Name: " + name 
			     + " and Root type: " + getObject(name)->ClassName());
  getHistogram(name)->Fill(number);
}

std::string AnalysisModule::getObjectName(const std::string& str) const
{
  std::string newName = currentFilter + str;

  // If the map has this already, great!
  if (objects.find(newName) == objects.end()) 
    {
      // If not, see if the nonfiltered version exists
      // and add a new member
      if (baseObjects.find(str) != baseObjects.end())
	{
	  addObjectClone(str, newName);
	}
      // Otherwise, it must be an error
      else
	{
	  throw std::out_of_range("Invalid key " + str + " passed to getObject()!");
	}
    }

  return newName;
}

void AnalysisModule::addObjectClone(const std::string& oldName, const std::string& newName) const
{
  TObject* clone = baseObjects.find(oldName)->second->Clone();
  
  // Rename it if possible
  auto named = dynamic_cast<TNamed*>(clone);

  if (named)
     {
        named->SetName(newName.c_str());
     }
   objects.insert({newName, clone});
}
