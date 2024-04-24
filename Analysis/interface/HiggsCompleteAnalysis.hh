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
        std::shared_ptr<Channel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() override {return channels;}
        std::vector<TH1*> getHistograms(const std::string& histType, const std::string& processName, const std::string& channel, bool scaleToExpected = false) override;
        TH1* getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false) const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
