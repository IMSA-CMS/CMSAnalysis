#ifndef SIMPLEPROCESS_HH
#define SIMPLEPROCESS_HH

#include <vector>
#include <memory>
#include "TH1.h"
#include "HistVariable.hh"

class SimpleProcess
{
    public:
        SimpleProcess(std::string processName, std::string filePathway, int processColor) : name(processName), filePath(filePathway), color(processColor) {}
	void addFile(std::string fileName, std::vector<HistVariable> histVariables);
	std::string getName() {return name;}
        TH1* getHist(std::string histType);
	TH1* getHistFromFile(std::string file, std::vector<HistVariable> histVariables, std::string histType);
        bool checkValidity(std::string file, std::vector<HistVariable> histVariables);

    private:
        std::string name;
        std::string filePath;
        std::vector<std::string> files;
        std::vector<std::vector<HistVariable>> histVariableVec;
        int color;
};

#endif
