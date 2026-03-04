#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
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

void addSingleProcess(std::shared_ptr<Process> process, std::string filePathway, std::string fileName, std::string crossSectionName,
                                std::shared_ptr<CrossSectionReader> crossReader, double luminosity,
                                std::shared_ptr<HistNameFinder> mappingLowMass, std::shared_ptr<HistNameFinder> mappingHighMass, bool isData = false,
                                double branchingRatioAdjustment = 1);

};

#endif