#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsChannelsAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include <vector>
#include <memory>
#include <iostream>

void Test() {
    std::shared_ptr<HiggsChannelsAnalysis> analysis = std::make_shared<HiggsChannelsAnalysis>();
    for(auto channel : analysis->getChannels()) {
        std::cout << channel->getName() << std::endl;
    }
}