#ifndef SIMPLEPROCESS_HH
#define SIMPLEPROCESS_HH

#include <vector>
#include "HistogramFinder.hh"
#include <memory>
#include "TH1.h"
#include "HistVariable.hh"

class SimpleProcess
{
    public:
        SimpleProcess(std::string processName, std::string filePathway, int processColor) : name(processName), filePath(filePathway), color(processColor) {}
	void addFile(std::string fileName, std::shared_ptr<HistogramFinder> histogramFinder);
	std::string getName() {return name;}
        TH1* getHist(HistVariable histType);
	TH1* getHistFromFile(std::string file, std::shared_ptr<HistogramFinder> histFinder, HistVariable histType);
        bool checkValidity(std::string file, std::shared_ptr<HistogramFinder> histFinder);

    private:
        std::string name;
        std::string filePath;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<HistogramFinder>> histFinders;
        int color;
};

#endif
