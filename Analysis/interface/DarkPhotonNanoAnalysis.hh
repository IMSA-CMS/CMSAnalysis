#ifndef DARKPHOTONNANOANALYSIS_HH
#define DARKPHOTONNANOANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonNanoAnalysis : public FullAnalysis
{
    public:
        DarkPhotonNanoAnalysis(int start, int end);

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif