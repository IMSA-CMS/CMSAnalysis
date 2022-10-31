#ifndef HIGGSGENCOMPARISONANALYSIS_HH
#define HIGGSGENCOMPARISONANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsGenComparisonAnalysis : public FullAnalysis
{
    public:
        HiggsGenComparisonAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
