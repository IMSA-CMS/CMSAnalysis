#ifndef DARKPHOTONINPUTANALYSIS_HH
#define DARKPHOTONINPUTANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonInputAnalysis : public FullAnalysis
{
    public:
        DarkPhotonInputAnalysis();

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif