#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/HistogramFinder.hh"
#include <memory>
#include <vector>
#include <string>
#include <iostream>

SingleProcess FullAnalysis::makeSingleProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::shared_ptr<HistogramFinder> fitHistogramFinder, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histogramFinder);
    auto fitInput = std::make_shared<RootFileInput>(filePathway + fitFileName, fitHistogramFinder);
    auto histEstimator = std::make_shared<FitEstimator>(massTarget, fitInput);
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity);
}

SingleProcess FullAnalysis::makeSignalProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histogramFinder);
    auto histEstimator = std::make_shared<WindowEstimator>(massTarget - (massTarget * .05), massTarget + (massTarget * .05));
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity);
}