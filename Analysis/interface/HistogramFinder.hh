#ifndef HISTOGRAMFINDER_HH
#define HISTOGRAMFINDER_HH

#include <unordered_map>
#include <string>
#include "HistVariable.hh"

class HistogramFinder
{
    public:
        HistogramFinder() {}
        void addHist(HistVariable histType, std::string histName);
        std::string getHistName(HistVariable histType);
        std::unordered_map<HistVariable, std::string> getMap() {return histNames;}

    private:
        std::unordered_map<HistVariable, std::string> histNames;
};

#endif