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

    void addParameterizations();

    void addSingleProcess(std::shared_ptr<Process> process, std::string filePathway, std::string fileName,
                          std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader,
                          std::shared_ptr<HistNameFinder> mappingLowMass,
                          std::shared_ptr<HistNameFinder> mappingHighMass, bool isData = false,
                          double branchingRatioAdjustment = 1);

  private:
    
};

#endif