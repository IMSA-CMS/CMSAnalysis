#ifndef HIGGSPLUSPLUSANALYSIS_HH
#define HIGGSPLUSPLUSANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsPlusPlusAnalysis : public FullAnalysis
{
    public:
        HiggsPlusPlusAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
