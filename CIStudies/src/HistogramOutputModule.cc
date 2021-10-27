#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include <iostream>
#include <stdexcept>
#include <typeinfo> // (TEMPORARY, TYPE CHECKING)

#include "TH1.h"
#include "TH2.h"

HistogramOutputModule::HistogramOutputModule(const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule) :
  //genSim(genSimModule),
  //reco(recoModule),
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

void HistogramOutputModule::setInput(std::shared_ptr<InputModule> iInput)
{
  Module::setInput(iInput);
  for (auto hist : histograms)
  {
    hist->setInput(iInput);
  }
}

void HistogramOutputModule::addObject(const std::string& name, TObject* obj)
{
  if (baseObjects.find(name) == baseObjects.end())
    baseObjects.insert({name, obj});
  else
    baseObjects[name] = obj;

  //std::cout << "Histogram added: " << name << '\n';

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
  throw std::runtime_error("Don't use this function!");

  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h)
{
  addObject(h->getFilteredName(), h->makeHistogram());
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h, std::string name)
{
  addObject(name, h->makeHistogram(name, name));
}

void HistogramOutputModule::fillHistogram(const std::string& name, std::vector<double> values, double weight)
{
  auto hist = getHistogram(name);
  if (!hist)
    throw std::runtime_error("Argument to getHistogram was not of TH1 type!  Name: " + name 
			     + " and Root type: " + getObject(name)->ClassName());

  if (auto hist2D = dynamic_cast<TH2*>(hist))  // If the hist is 2D hist
  {
    if (values.size() % 2 == 1) // If the size of values is odd
    {
      throw std::runtime_error("Number of values in values vector is not even!");
    }

    for (int x = 0; x < static_cast<int>(values.size()); x += 2)
    {
      // Since x and y values are just inputed into the values vector as {x, y, x, y, x, y, ...}
      // The for loop loops through every other element in values (i.e. every x value)
      // The y value is then the element immediately after the x element
      hist2D->Fill(values[x], values[x + 1], weight);
    }
  }

  else  // If the hist is 1D hist
  {
    for(double currentNum: values)
    {
      hist->Fill(currentNum, weight);
    }
  }
}

std::string HistogramOutputModule::getObjectName(const std::string& str) const
{
  std::string newName = getFilter() + str;
  //std::cout << "Histogram Got: " << newName << '\n';

  //for (auto pair : objects)
  //{
    //std::cout << "test\n";
    //std::cout << pair.first << '\n';
  //}

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

bool HistogramOutputModule::process()
{
  // std::cout << "Process\n";

  std::string massBin = getFileParams().getMassRange();
  std::string helicity = getFileParams().getHelicity();

  //std::cerr << "plot contrivance" << std::endl;

  double eventWeight = 1.00;
  /*if (helicity == "LR")
    {
      eventWeight = lrWeighting->getLRWeight();
    }
  if (helicity == "RL")
    {
      eventWeight = lrWeighting->getRLWeight();
    }
  */
  //std::cerr << "MS paint drawing" << std::endl;


  for (auto hist : histograms)
  {
    bool draw = hist->shouldDraw(); // call the shouldDraw function so we can call process on the FilterModules
    // std::cout << "shouldDraw returns: " << draw << '\n';
    
    // If the mass bin is a new mass bin, then make the histograms for that mass bin
    if (isNewMassBin(massBin))
      {
        auto weight = weighting->getWeight();
        auto fileKey = getFileParams().getFileKey();
    
        massBins[massBin] = weight;
        fileKeys[massBin] = fileKey;

        // std::cout << "New Mass Bin: " << massBin << '\n';
        // std::cout << "New Histogram Made: " << hist->getFilteredName() + massBin << '\n';
        //std::cout << "Weight " << weight << '\n';
    //    makeHistogram(hist->getFilteredName() + massBin, hist->getFilteredName() + massBin, hist->getNBins(), hist->getMinimum(), hist->getMaximum());
    //    addMassBinObject(hist->getFilteredName(), massBin);
        }

    //std::cerr << "Shrek reference" << std::endl;

    // If the histogram with mass bin doesn't exist, make it
    if (baseObjects.find(hist->getFilteredName() + massBin) == baseObjects.end())
      {
        // Uncommented two lines to debug
        // std::cout << "Name: " << hist->getName() << '\n';
        // std::cout << "FilteredName: " << hist->getFilteredName() << '\n';
        // std::cout << "Mass Bin: " << massBin << '\n';
        // std::cout << "Histogram missing and made (with Mass Bin): " << hist->getFilteredName() + massBin << '\n';
        makeHistogram(hist, hist->getFilteredName() + massBin);
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
        makeHistogram(hist, hist->getFilteredName()); 
      }

    // Fill the histogram if the filter string isn't empty
    if (hist->getName() != hist->getFilteredName()) // If the filter string is empty, then the name and the filtered name should be the same
      {
         //std::cout << "Name: " << hist->getName() << '\n';
         //std::cout << "FilteredName: " << hist->getFilteredName() << '\n';
         //std::cout << "Mass Bin: " << massBin << '\n';
         //std::cout << "Histogram missing and made: " << hist->getFilteredName() << '\n';
        makeHistogram(hist, hist->getFilteredName()); 
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
      // std::cout << "PAIR: " << pair.first << '\n';
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
	        std::cout << "Scale------ (massBin.second): " << massBin.second << "\n";
              }

              // for (int i = 1; i < getHistogram(pair.first)->GetNbinsX() ; ++i)
              // {
                // getHistogram(pair.first)->AddBinContent(i, getHistogram(pair.first + massBin.first)->GetBinContent(i));
              // }
              getHistogram(pair.first)->Add(getHistogram(pair.first + massBin.first));

              //std::cout << pair.first + massBin.first << '\n';

              for (auto pair : objects) 
              {
                // Temporary testing, comment when done[09/25/21]
                // std::cout << "Type: " << typeid(pair.first).name() << "\n";
                // std::cout << pair.first << "\n";
              }

              std::cout << "Erasing " << pair.first + massBin.first << "\n";
            
              auto len = (pair.first).length();
              // std::cout << len << std::endl;

              // Quick hack: if we erase the filtered things in objects will it work with filters?
              for (auto pair2 : objects)
              {
                auto len2 = pair2.first.length();
                /* std::cout << pair2.first << std::endl;
                if(len2 >= len) {
                  std::cout << pair2.first.substr(len2 - len) << std::endl;
                } */

                if(len2 >= len && (pair2.first.substr(len2 - len) + massBin.first) 
                  == (pair.first + massBin.first)) {
                  // Should only be triggered by filtered things
                  // std::cout << len2 - len << "!\n";
                  objects.erase(pair2.first + massBin.first);
                  // std::cout << pair2.first << "!\n";
                }
              }

              // IF YOU UNCOMMENT THIS IT WILL EXPLODE SO PLEASE DONT
              // I KNOW THIS IS BAD PRACTICE AND I DONT CARE
              // delete getObject(pair.first + massBin.first);   // Remove the extra "name+M___" histograms from the root file
              // Should only work for non-filtered things: therefore everything necessary is erased
              objects.erase(pair.first + massBin.first);      // Remove the pointer in the objects map
              baseObjects.erase(pair.first + massBin.first);  // Remove the pointer in the baseObjects map

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

