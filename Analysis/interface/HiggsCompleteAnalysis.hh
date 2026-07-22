#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "FullAnalysis.hh"
#include <string>
#include <tuple>
#include <vector>

class HiggsCompleteAnalysis : public FullAnalysis
{
  public:
    HiggsCompleteAnalysis();
    double getBranchingRatio(const std::string &channel) const;

    static const std::vector<std::string> genSimDecays;
    static const std::vector<std::string> recoDecays;
    static const std::vector<int> massTargets;

    std::vector<std::string> getSystematics() const override;

    void addSingleProcess(std::shared_ptr<Process> process, std::string filePathway, std::string fileName,
                          std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader,
                          std::shared_ptr<HistNameFinder> mappingLowMass,
                          std::shared_ptr<HistNameFinder> mappingHighMass, bool isData = false,
                          double branchingRatioAdjustment = 1);

  private:
    // Returns a tuple of (HistVar, channel, genSim, paramName)
    static std::tuple<HistVariable, std::string, std::string, std::string> parseSignalParamFuncName(
        const std::string &name);
    // Returns a tuple of (HistVar, channel, bgName)
    static std::tuple<HistVariable, std::string, std::string> parseBgFuncName(const std::string &name);
};

#endif