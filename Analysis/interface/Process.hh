#ifndef PROCESS_HH
#define PROCESS_HH

#include "SingleProcess.hh"
#include "HistVariable.hh"
#include "TH2F.h"
#include <string>
#include <vector>
#include "MultiSystematic.hh"

class TH1;
class Systematic;
class Process
{
    //MultiSystematic systematics;
    public:
        Process(std::string name, int color) : name(name), color(color), systematics("totalSystematics") {}
        std::string getName() const {return name;}
        int getColor() const {return color;}
        int getNEvents();
        std::vector<SingleProcess> getProcesses() const {return processes;}
        //Gets the hists frm all the singleProcesses as one hist
        TH1* getHist(const HistVariable& histType, bool scaleToExpected = false) const;
        TH2* get2DHist(const HistVariable& histType) const;
        TH1* getSingleProcessHist(const HistVariable& histType, const std::string& singleProcessName, bool scaleToExpected = false) const;
        const SingleProcess& getSingleProcess(const std::string& singleProcessName) const;
        void addProcess(SingleProcess process);
        //Gets the total yield of all singleProcesses
        double getYield(const HistVariable& dataType) const;
        //Returns table format data of all sinlleProcesses
        //std::vector<std::vector<std::string>> getData() const;
        void addSystematic(std::shared_ptr<Systematic> systematic);
        std::pair<TH1*, TH1*> getSystematicHist(HistVariable histType, bool scaleToExpected = false);

        static std::pair<TH1*, TH1*> combineSystematics(std::vector<std::shared_ptr<Process>> processes, TH1* original);

        std::shared_ptr<Systematic> calcSystematic(HistVariable histType, std::string systematicName, bool isShape);
        std::shared_ptr<Systematic> calcRateSystematic(HistVariable histType, std::string systematicName);
        std::shared_ptr<Systematic> calcShapeSystematic(HistVariable histType, std::string systematicName);
    private:
        const std::string name;
        const Color_t color;
        std::vector<SingleProcess> processes;
        MultiSystematic systematics;

};

#endif
