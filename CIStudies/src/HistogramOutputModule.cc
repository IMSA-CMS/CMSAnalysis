#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include <iostream>
#include <stdexcept>

#include "TH1.h"

HistogramOutputModule::HistogramOutputModule(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule) :
  genSim(genSimModule),
  reco(recoModule),
  weighting(weightingModule),
  lrWeighting(lrWeightModule)
{
}

void HistogramOutputModule::writeAll()
{
  // Check if any baseObjects have not been copied to objects yet,
  // and fill them into the main map if not
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
  
  // Then write all the objects to file
  for (auto& entry : objects)
    {
      entry.second->Write();
    }
}

void HistogramOutputModule::addObject(const std::string& name, TObject* obj)
{
  if (baseObjects.find(name) == baseObjects.end())
    baseObjects.insert({name, obj});
  else
    baseObjects[name] = obj;

  // If this is called during finalize(), it needs to be added explicitly/
  // The filter name is set properly during calls to finalize(), so 
  // this will work.
  if (isDone())
    addObjectClone(name, getFilter() + name);
}

void HistogramOutputModule::addMassBinObject(std::string name, std::string massbin)
{
  if (massBinMap.find(name) == massBinMap.end())  // 
  {
    // std::cout << "Added Mass Bin: " << name << ", " << massbin << '\n';
    std::vector<std::string> v;           // Create a vector
    v.push_back(massbin);                 // Add the mass bin string to the vector
    massBinMap.insert({name, v});
  }
  else
  {
    massBinMap[name].push_back(massbin);    // Add the mass bin string to the vector
  }
}

void HistogramOutputModule::makeHistogram(const std::string& name, const std::string& title, int nbins, double min, double max)
{
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h)
{
  const std::string& name = h->getFilteredName();
  // std::cout << "makeHistogram Name: " << name << '\n';
  auto title = name;
  int nbins = h->getNBins();
  double min = h->getMinimum();
  double max = h->getMaximum();
  
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void HistogramOutputModule::fillHistogram(const std::string& name, double number, double weight)
{
  auto hist = getHistogram(name);
  if (!hist)
    throw std::runtime_error("Argument to getHistogram was not of TH1 type!  Name: " + name 
			     + " and Root type: " + getObject(name)->ClassName());
  hist->Fill(number, weight);
}

std::string HistogramOutputModule::getObjectName(const std::string& str) const
{
  std::string newName = getFilter() + str;
  // std::cout << "Histogram Got: " << newName << '\n';

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

void HistogramOutputModule::addObjectClone(const std::string& oldName, 
					   const std::string& newName) const
{
  // Clone the object
  TObject* clone = baseObjects.find(oldName)->second->Clone();
  
  // Rename it if possible
  auto named = dynamic_cast<TNamed*>(clone);
  if (named)
     {
        named->SetName(newName.c_str());
     }

  // Insert it into the map
  objects.insert({newName, clone});
}

bool HistogramOutputModule::process(const edm::EventBase& event)
{
  // std::cout << "Process\n";

  std::string massBin = getFileParams().getMassRange();
  std::string helicity = getFileParams().getHelicity();

  //std::cerr << "plot contrivance" << std::endl;

  double eventWeight = 1.00;
  if (helicity == "LR")
    {
      eventWeight = lrWeighting->getLRWeight();
    }
  if (helicity == "RL")
    {
      eventWeight = lrWeighting->getRLWeight();
    }

  //std::cerr << "MS paint drawing" << std::endl;

  for (auto hist : histograms)
  {
    bool draw = hist->shouldDraw(event); // call the shouldDraw function so we can call process on the FilterModules
    // std::cout << "shouldDraw returns: " << draw << '\n';
    
    // If the mass bin is a new mass bin, then make the histograms for that mass bin
    if (isNewMassBin(massBin))
      {
        auto weight = weighting->getWeight();
        auto fileKey = getFileParams().getFileKey();
    
        massBins[massBin] = weight;
        fileKeys[massBin] = fileKey;

    //    // std::cout << "New Mass Bin: " << massBin << '\n';
    //    // std::cout << "New Histogram Made: " << hist->getFilteredName() + massBin << '\n';
    //    makeHistogram(hist->getFilteredName() + massBin, hist->getFilteredName() + massBin, hist->getNBins(), hist->getMinimum(), hist->getMaximum());
    //    addMassBinObject(hist->getFilteredName(), massBin);
        }

    //std::cerr << "Shrek reference" << std::endl;

    // If the histogram with mass bin doesn't exist, make it
    if (baseObjects.find(hist->getFilteredName() + massBin) == baseObjects.end())
      {
        // std::cout << "Name: " << hist->getName() << '\n';
        // std::cout << "FilteredName: " << hist->getFilteredName() << '\n';
        // std::cout << "Mass Bin: " << massBin << '\n';
        // std::cout << "Histogram missing and made (with Mass Bin): " << hist->getFilteredName() + massBin << '\n';
        makeHistogram(hist->getFilteredName() + massBin, hist->getFilteredName() + massBin, hist->getNBins(), hist->getMinimum(), hist->getMaximum()); 
        addMassBinObject(hist->getFilteredName(), massBin);
      }

    //std::cerr << "2012 apocalypse" << std::endl;

    // If the histogram without mass bin doesn't exist, make it
    if (baseObjects.find(hist->getFilteredName()) == baseObjects.end())
      {
        // std::cout << "Name: " << hist->getName() << '\n';
        // std::cout << "FilteredName: " << hist->getFilteredName() << '\n';
        // std::cout << "Mass Bin: " << massBin << '\n';
        // std::cout << "Histogram missing and made (without Mass Bin): " << hist->getFilteredName() << '\n';
        makeHistogram(hist->getFilteredName(), hist->getFilteredName(), hist->getNBins(), hist->getMinimum(), hist->getMaximum()); 
      }

    // Fill the histogram if the filter string isn't empty
    if (hist->getName() != hist->getFilteredName()) // If the filter string is empty, then the name and the filtered name should be the same
      {
         //std::cout << "Name: " << hist->getName() << '\n';
         //std::cout << "FilteredName: " << hist->getFilteredName() << '\n';
         //std::cout << "Mass Bin: " << massBin << '\n';
         //std::cout << "Histogram missing and made: " << hist->getFilteredName() << '\n';
        makeHistogram(hist->getFilteredName(), hist->getFilteredName(), hist->getNBins(), hist->getMinimum(), hist->getMaximum()); 
      }

    //std::cerr << "value = " << hist->value() << std::endl;

    // Fill the histogram if shouldDraw(event) (draw) returns true
    //std::cerr << "dunkin donuts" << std::endl;
    if (draw)
      {
        // std::cout << "Histogram Filled: " << hist->getFilteredName() + massBin << '\n' << "Value: " << hist->value() << '\n';
        fillHistogram(hist->getFilteredName() + massBin, hist->value(), eventWeight);
      }
  }
  //std::cerr << "kumon: math, reading, success" << std::endl;
  return true;
}

void HistogramOutputModule::finalize()
{
  // std::cout << "Finalize\n";

  for (auto pair : massBinMap)
  {
    // std::cout << pair.first << '\n';
    // std::cout << "Pair First: " << pair.first << "\t" << "Pair Second: " << pair.second << "\n";

    for (auto massBin : massBins)
      {
        // std::cout << "Mass Bin: " << massBin.first << '\n';
        auto fileKey = fileKeys[massBin.first];
        auto eventCount = getEventCount(fileKey);

        // std::cout << "Pair Second Size: " << pair.second.size() << "\n";

        for (auto bin : pair.second)
        {
          // std::cout << "Bin: " << bin << "\t" << "Event Count: " << eventCount << "\n";
          if (bin == massBin.first && eventCount != 0)
          {
            if (massBin.second != 0)
            {
              getHistogram(pair.first + bin)->Scale(massBin.second / eventCount);  // massBin.second is the scale
            }

            // for (int i = 1; i < getHistogram(pair.first)->GetNbinsX() ; ++i)
            // {
              // getHistogram(pair.first)->AddBinContent(i, getHistogram(pair.first + massBin.first)->GetBinContent(i));
            // }
            getHistogram(pair.first)->Add(getHistogram(pair.first + massBin.first));
          }
        }
      }    
  }
}

bool HistogramOutputModule::isNewMassBin(const std::string mass)
{
  for (auto massBin : massBins)
    {
      if (mass == massBin.first)
	{
	  return false;
	}
    }
  
  return true;
}

