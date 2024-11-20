#ifndef DARKPHOTONMCANALYSIS_HH
#define DARKPHOTONMCANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonMCAnalysis : public FullAnalysis
{
    public:
        DarkPhotonMCAnalysis(const std::string filePath, const std::string crossSectionPath);

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif