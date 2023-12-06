#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/SimpleEstimator.hh"
#include <memory>
#include <vector>
#include <string>
#include <iostream> 

SingleProcess FullAnalysis::makeSingleProcess(std::vector<HistVariable> histVariables, std::vector<HistVariable> fitHistVariables, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histVariables);
    auto fitInput = std::make_shared<RootFileInput>(filePathway + fitFileName, fitHistVariables);
    auto histEstimator = std::make_shared<FitEstimator>(massTarget, fitInput);
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity, corrections);
}

//This is called SignalProcess but just use it as long as you don't need to use scaleToExpected or don't have fit histograms.
SingleProcess FullAnalysis::makeSignalProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histVariables);
    auto histEstimator = std::make_shared<SimpleEstimator>();
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity, corrections);
}
