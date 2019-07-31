#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TGraphErrors.h>
#include <vector>
#include <TF1.h>

ResolutionModule::ResolutionModule(const MatchingModule& matchingModule, int minPt, int maxPt, int pTInterval) :
  matching(matchingModule),
  minPtCut(minPt),
  maxPtCut(maxPt),
  interval(pTInterval)
{
}

bool ResolutionModule::process(const edm::EventBase& event)
{
  auto bestPairs = matching.getMatchingBestPairs();

  for (auto matchingPair : bestPairs.getPairs())
    {
      double genSimPt = matchingPair.getGenParticle()->pt();
      std::string pTBin = pickPtBin(genSimPt);
      fillHistogram("ResolutionHistBin" + pTBin, matchingPair.getPtError());
    }

  return true;
}

void ResolutionModule::initialize()
{
  auto numberOfBins = getNumberOfBins();


  for(int i = 0; i < numberOfBins; ++i)
    {
      const std::string resolutionStr = "Resolution";
      
      const int histBins = 100;
      
      const double minPtError = -0.5;
      const double maxPtError = 0.5;

      makeHistogram(("ResolutionHistBin" + std::to_string(i * interval + minPtCut)), resolutionStr, histBins, minPtError, maxPtError);
    }
}

void ResolutionModule::finalize()
{
  auto pTErrorMeanHist = makeTGraphErrors("pTErrorMeanHist", getNumberOfBins());
  auto pTErrorStdDevHist = makeTGraphErrors("pTErrorStdDevHist", getNumberOfBins());

  const int xError = interval / 2;

  int index = 0;
  int median = minPtCut + interval / 2;

  for (int i = minPtCut; i < maxPtCut; i += interval)
    {
      auto errorMean = getErrorValues(getHistogram("ResolutionHistBin" + std::to_string(i)), "mean");
      auto errorStdDev = getErrorValues(getHistogram("ResolutionHistBin" + std::to_string(i)), "standard deviation");

      pTErrorMeanHist->SetPoint(index, median, errorMean.first);
      pTErrorMeanHist->SetPointError(index, xError, errorMean.second);

      pTErrorStdDevHist->SetPoint(index, median, errorStdDev.first);
      pTErrorStdDevHist->SetPointError(index, xError, errorStdDev.second);

      ++index;
      median += interval;
    }

  AnalysisModule::finalize();
}

std::string ResolutionModule::pickPtBin(double pT) const
{
  int pTBin = static_cast<int>(pT) / interval * interval; //mass is invariantMass floored to the highest multiple of interval
  if (pTBin < minPtCut)
    {
      pTBin = minPtCut;
    }
  else if (pTBin > maxPtCut - interval)
    {
      pTBin = maxPtCut - interval;
    }

  return std::to_string(pTBin);
}

const int ResolutionModule::getNumberOfBins() const
{
  return (maxPtCut - minPtCut) / interval;
}

TGraphErrors* ResolutionModule::makeTGraphErrors(std::string name, int numberOfBins)
{
  auto hist = new TGraphErrors(numberOfBins);
  hist->SetTitle(name.c_str());
  hist->SetName(name.c_str());
  addObject(name, hist);

  return hist;
}

std::pair<double, double> ResolutionModule::getErrorValues(TH1* histogram, std::string errorType)
{
  const double minPtError = -0.5;
  const double maxPtError = 0.5;

  if (histogram->GetEntries() > 0)
    {
      histogram->Fit("gaus", "", "", minPtError, maxPtError);

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

