#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"

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

HistogramOutputModule::HistogramOutputModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const LRWeightModule& lrWeightModule) :
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

void HistogramOutputModule::makeHistogram(const std::string& name, const std::string& title, int nbins, double min, double max)
{
  auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
  addObject(name, newHist);
}

void HistogramOutputModule::makeHistogram(HistogramPrototype* h)
{
  const std::string& name = h->getName();
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
  getHistogram(name)->Fill(number, weight);
}

std::string HistogramOutputModule::getObjectName(const std::string& str) const
{
  std::string newName = getFilter() + str;

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

void HistogramOutputModule::initialize()
{
  for (HistogramPrototype* hist : histograms)
  {
    makeHistogram(hist);
  }
}

bool HistogramOutputModule::process(const edm::EventBase& event)
{
  std::string massBin = getFileParams().getMassRange();
  std::string helicity = getFileParams().getHelicity();

  double eventWeight = 1.00;
  if (helicity == "LR")
    {
      eventWeight = lrWeighting.getLRWeight();
    }
  if (helicity == "RL")
    {
      eventWeight = lrWeighting.getRLWeight();
    }

  if (isNewMassBin(massBin))
    {
      auto weight = weighting.getWeight();
      auto fileKey = getFileParams().getFileKey();

      massBins[massBin] = weight;
      fileKeys[massBin] = fileKey;

      for (HistogramPrototype* hist : histograms)
      {
        makeHistogram(hist->getName() + massBin, hist->getName() + massBin, hist->getNBins(), hist->getMinimum(), hist->getMaximum());
      }
    }

  for (HistogramPrototype* hist : histograms)
  {
    if (hist->shouldDraw())
      {
        fillHistogram(hist->getName() + massBin, hist->value(), eventWeight);
      }
  }

  return true;
}

void HistogramOutputModule::finalize()
{
  for (HistogramPrototype* hist : histograms)
  {
    for (auto massBin : massBins)
      {
        auto fileKey = fileKeys[massBin.first];
        auto eventCount = getEventCount(fileKey);

        // std::cout << eventCount << std::endl;

	getHistogram(hist->getName() + massBin.first)->Scale(massBin.second / eventCount);
        // getHistogram("Invariant Mass GenSim" + massBin.first)->Scale(massBin.second / eventCount);
        // getHistogram("Invariant Mass Reco" + massBin.first)->Scale(massBin.second / eventCount) ;
        // getHistogram("Transverse Momentum GenSim" + massBin.first)->Scale(massBin.second / eventCount);
        // getHistogram("Transverse Momentum Reco" + massBin.first)->Scale(massBin.second / eventCount) ;
      }


    for (int i = 1; i < hist->getNBins() ; ++i)
      {
        for (auto massBin : massBins)
	  {
	    getHistogram(hist->getName())->AddBinContent(i, getHistogram(hist->getName() + massBin.first)->GetBinContent(i));
	    // getHistogram("GenSim Invariant Mass Pasted")->AddBinContent(i, getHistogram("Invariant Mass GenSim" + massBin.first)->GetBinContent(i));
  	    // getHistogram("Reco Invariant Mass Pasted")->AddBinContent(i, getHistogram("Invariant Mass Reco" + massBin.first)->GetBinContent(i));
	    // getHistogram("GenSim Transverse Momentum Pasted")->AddBinContent(i, getHistogram("Transverse Momentum GenSim" + massBin.first)->GetBinContent(i));
	    // getHistogram("Reco Transverse Momentum Pasted")->AddBinContent(i, getHistogram("Transverse Momentum Reco" + massBin.first)->GetBinContent(i));

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

