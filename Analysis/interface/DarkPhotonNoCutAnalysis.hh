#ifndef DARKPHOTONNOCUTANALYSIS_HH
#define DARKPHOTONNOCUTANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonNoCutAnalysis : public FullAnalysis
{
    public:
        DarkPhotonNoCutAnalysis(const std::string filePath);

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif