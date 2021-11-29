#ifndef EFFICIENCYMODULE_HH
#define EFFICIENCYMODULE_HH
#include "AnalysisModule.hh"
#include "WeightingModule.hh"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

class EfficiencyModule : public AnalysisModule
{
    public:
    EfficiencyModule(const std::shared_ptr<WeightingModule> WeightingModule);
    void finalize() override;
    virtual void writeAll() override;

    protected:
    void incrementCounter(std::string name, double increment = 1);
    double getCounter(std::string name) const;
    double getEfficiency(std::string name) const;
    bool process() override;
    virtual void doCounters() = 0; 

    private:
    std::unordered_map<std::string, std::vector<double>> counters;
    std::vector<double> total;
    bool isNewFile()const;
    std::string currentFileKey;
    std::vector<double> weights;
    const std::shared_ptr<WeightingModule> weighting;

};

#endif