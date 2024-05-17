#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsCompleteAnalysis : public FullAnalysis
{
public:
    HiggsCompleteAnalysis();
    // bool checkChannelName(std::string channelName, double massTarget);

    // TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false, std::string channelName = "") const override;
};

#endif
