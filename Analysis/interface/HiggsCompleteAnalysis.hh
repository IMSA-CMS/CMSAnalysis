#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include <vector>

class HiggsCompleteAnalysis : public FullAnalysis
{
  public:
    HiggsCompleteAnalysis();
    double getBranchingRatio(const std::string &channel);

    static const std::vector<std::string> genSimDecays;
    static const std::vector<std::string> recoDecays;
    static const std::vector<int> massTargets;
    static const std::vector<std::string> systematics;

    // bool checkChannelName(std::string channelName, double massTarget);

    // TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false,
    // std::string channelName = "") const override;
};

#endif