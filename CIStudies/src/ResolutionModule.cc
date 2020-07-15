#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TGraphErrors.h>
#include <vector>
#include <TF1.h>

ResolutionModule::ResolutionModule(const MatchingModule& matchingModule, std::string bin, int min, int max, int intervalSize) :
  matching(matchingModule),
  binType(bin),
  minCut(min),
  maxCut(max),
  interval(intervalSize)
{
}

bool ResolutionModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING ResolutionModule" << std::endl;
  auto bestPairs = matching.getMatchingBestPairs();
  fillError(bestPairs);
  //std::cerr << "EXITING ResolutionModule" << std::endl;
  return true;
}

void ResolutionModule::initialize()
{
  auto numberOfBins = getNumberOfBins();


  for(int i = 0; i < numberOfBins; ++i)
    {
      const std::string resolutionStr = "Resolution";
      
      const int histBins = 100;

      makeHistogram((binType + "ResolutionHistBin" + std::to_string(i * interval + minCut)), resolutionStr, histBins, minError, maxError);
    }
}

void ResolutionModule::finalize()
{
  auto errorMeanHist = makeTGraphErrors(binType + "ErrorMeanHist", getNumberOfBins());
  auto errorStdDevHist = makeTGraphErrors(binType + "ErrorStdDevHist", getNumberOfBins());

  const int xError = interval / 2;

  int index = 0;
  int median = minCut + interval / 2;

  for (int i = minCut; i < maxCut; i += interval)
    {
      auto errorMean = getErrorValues(getHistogram(binType + "ResolutionHistBin" + std::to_string(i)), "mean");
      auto errorStdDev = getErrorValues(getHistogram(binType + "ResolutionHistBin" + std::to_string(i)), "standard deviation");

      errorMeanHist->SetPoint(index, median, errorMean.first);
      errorMeanHist->SetPointError(index, xError, errorMean.second);

      errorStdDevHist->SetPoint(index, median, errorStdDev.first);
      errorStdDevHist->SetPointError(index, xError, errorStdDev.second);

      ++index;
      median += interval;
    }

  addObject(binType + "ErrorMeanHist", errorMeanHist);
  addObject(binType + "ErrorStdDevHist", errorStdDevHist);

  AnalysisModule::finalize();
}

std::string ResolutionModule::pickBin(double value) const
{
  int bin = static_cast<int>(value) / interval * interval; //mass is invariantMass floored to the highest multiple of interval
  if (bin < minCut)
    {
      bin = minCut;
    }
  else if (bin > maxCut - interval)
    {
      bin = maxCut - interval;
    }

  return std::to_string(bin);
}

const int ResolutionModule::getNumberOfBins() const
{
  return (maxCut - minCut) / interval;
}

TGraphErrors* ResolutionModule::makeTGraphErrors(std::string name, int numberOfBins)
{
  auto hist = new TGraphErrors(numberOfBins);
  hist->SetTitle(name.c_str());
  hist->SetName(name.c_str());

  return hist;
}

std::pair<double, double> ResolutionModule::getErrorValues(TH1* histogram, std::string errorType)
{
  if (histogram->GetEntries() > 0)
    {
      histogram->Fit("gaus", "q", "", minError, maxError); //q gets rid of table output to screen

      auto gaus = histogram->GetFunction("gaus");

      if (errorType == "mean")
	{
	  std::pair<double, double> mean(gaus->GetParameter(1), gaus->GetParError(1));
	  return mean;
	}

      else if (errorType == "standard deviation")
	{
	  std::pair<double, double> stdDev(gaus->GetParameter(2), gaus->GetParError(2));
	  return stdDev;
	}
      else 
	{
	  throw std::runtime_error("Invalid Error Type");
	}
    }

  else 
    {
      std::pair<double, double> empty(0,0);
      return empty;
    }
}

