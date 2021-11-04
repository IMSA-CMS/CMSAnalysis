#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/FilestripModule.hh"

using std::make_shared;

Analyzer filestripAnalysis()
{
    Analyzer analyzer;

    auto filestripMod = make_shared<FilestripModule>();

    analyzer.addAnalysisModule(filestripMod);
    
    return analyzer;
}