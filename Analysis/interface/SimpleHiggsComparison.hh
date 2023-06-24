#ifndef SIMPLEHIGGSCOMPARISON_HH
#define SIMPLEHIGGSCOMPARISON_HH

#include "SimpleAnalysis.hh"
#include "SimpleChannel.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class SimpleHiggsComparison : public SimpleAnalysis
{
    public:
        SimpleHiggsComparison();
        std::shared_ptr<SimpleChannel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<SimpleChannel>> getChannels() {return channels;}

    private:
        std::vector<std::shared_ptr<SimpleChannel>> channels;
};

#endif
