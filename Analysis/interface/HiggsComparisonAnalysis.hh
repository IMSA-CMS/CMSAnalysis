#ifndef HIGGSCOMPARISONANALYSIS_HH
#define HIGGSCOMPARISONANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsComparisonAnalysis : public FullAnalysis
{
    public:
        HiggsComparisonAnalysis();
        TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false, std::string channelName = "") const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
