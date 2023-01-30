#ifndef HIGGSCUTSANALYSIS_HH
#define HIGGSCUTSANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsCutsAnalysis : public FullAnalysis
{
    public:
        HiggsCutsAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
	std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
