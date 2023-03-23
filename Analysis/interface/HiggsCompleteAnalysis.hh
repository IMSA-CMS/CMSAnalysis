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
	    std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}
        TH1* getHiggsHist(std::string histType, bool scaleToExpected = false) const;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
