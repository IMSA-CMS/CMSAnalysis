#ifndef DYCOMPARISONANALYSIS_HH
#define DYCOMPARISONANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DYComparisonAnalysis : public FullAnalysis
{
    public:
        DYComparisonAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
        std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif



