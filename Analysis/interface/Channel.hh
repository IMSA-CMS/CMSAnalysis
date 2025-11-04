#ifndef CHANNEL_HH
#define CHANNEL_HH

#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "Process.hh"
#include "TH1.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Process;
class THStack;
class Systematic;
class Channel
{
  public:
    enum class Label
    {
        Signal,
        Data,
        Background,
    };

    // Channel(std::string name, std::string iYAxisName, std::vector<std::shared_ptr<Process>> iProcesses);
    Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses);
    // Gets data from all processes in the format needed to make table
    std::vector<double> getYields(HistVariable dataType) const;
    std::vector<std::string> getNames() const;
    std::string getName() const
    {
        return name;
    }
    std::vector<std::string> getNamesWithLabel(Label label);
    // Makes stacked histogram
    THStack *getStack(HistVariable histType, std::optional<Label> label = {}, bool scaleToExpected = false,
                      int rebinConstant = 1) const;

    std::vector<TH1 *> getHists(HistVariable histType, std::optional<Label> label = {},
                                bool scaleToExpected = false) const;

    const std::shared_ptr<Process> findProcess(std::string processName) const;

    void labelProcess(Label label, std::string processName);

    void labelProcess(Label label, std::shared_ptr<Process> process);

    void addProcessLabel(Label label, std::vector<std::shared_ptr<Process>> processes);

    void makeDatacard(HistVariable histType);

    void CombineDatacard(HistVariable histType);

    void addGlobalSystematic(Systematic &systematic);

    std::vector<std::shared_ptr<Process>> getProcesses()
    {
        return processes;
    }

    static TH1 *combineHists(const std::vector<TH1 *> &hists);

  private:
    std::string name;
    // std::string yAxisName;
    std::vector<std::shared_ptr<Process>> processes;
    std::unordered_map<Label, std::vector<std::shared_ptr<Process>>> map;
};

#endif
