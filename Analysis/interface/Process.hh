#ifndef PROCESS_HH
#define PROCESS_HH

#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "HistVariable.hh"
#include "MultiSystematic.hh"
#include "SingleProcess.hh"
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class TH1;
class Systematic;
class Process
{
    // MultiSystematic systematics;
  public:
    Process(std::string name, Color_t color) : name(std::move(name)), color(color), systematics("totalSystematics") {}
    std::string getName() const { return name; }
    int getColor() const { return color; }
    int getNEvents();
    std::vector<SingleProcess> getProcesses() const { return processes; }

    // Gets the hists from all the singleProcesses as one hist
    TH1 *getHist(const HistVariable &histType, bool scaleToExpected = false) const;
    TH2 *get2DHist(const HistVariable &histType) const;
    TH1 *getSingleProcessHist(const HistVariable &histType, const std::string &singleProcessName, 
                              bool scaleToExpected = false) const;
    const SingleProcess &getSingleProcess(const std::string &singleProcessName) const;
    void addProcess(SingleProcess process);
    // Gets the total yield of all singleProcesses
    double getYield(const HistVariable &dataType) const;
    // Returns table format data of all singleProcesses 
    // std::vector<std::vector<std::string>> getData() const;
    void addSystematic(std::shared_ptr<Systematic> systematic);
    std::pair<TH1 *, TH1 *> getSystematicHist(HistVariable histType, bool scaleToExpected = false);

    std::pair<TH1 *, TH1 *> combineSystematics(std::vector<std::shared_ptr<Process>> processes, TH1 *original,
                                               HistVariable histVar);

    std::shared_ptr<Systematic> calcSystematic(HistVariable histType, std::string systematicName, bool isShape);
    std::shared_ptr<Systematic> calcRateSystematic(HistVariable histType, std::string systematicName);
    std::shared_ptr<Systematic> calcShapeSystematic(HistVariable histType, std::string systematicName);
    std::optional<FitFunction> getPlot(const HistVariable &histType);
    void setPlot(const HistVariable &histType, FitFunction plot);

  private:
    const std::string name;
    const Color_t color;
    std::vector<SingleProcess> processes;
    MultiSystematic systematics;
    // Fitted plots
    std::vector<std::tuple<HistVariable, FitFunction>> plots;
};

#endif
