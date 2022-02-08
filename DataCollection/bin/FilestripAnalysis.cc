#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/FilestripModule.hh"

using std::make_shared;

Analyzer filestripAnalysis()
{
    Analyzer analyzer;

    //auto filestripMod = make_shared<FilestripModule>();

    //analyzer.addAnalysisModule(filestripMod);
    
    return analyzer;
}