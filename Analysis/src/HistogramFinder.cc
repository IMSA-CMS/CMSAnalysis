#include <unordered_map>
#include <string>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HistogramFinder.hh"
#include <iostream>

void HistogramFinder::addHist(HistVariable histType, std::string histName) {
    histNames[histType] = histName;
}

std::string HistogramFinder::getHistName(HistVariable histType) {
    std::unordered_map<HistVariable, std::string>::const_iterator iter = histNames.find(histType);
    if(iter == histNames.end()) {
        throw std::runtime_error("HistType doesn't have an associated hist");
    }
    else {
        return iter->second;
    }
}