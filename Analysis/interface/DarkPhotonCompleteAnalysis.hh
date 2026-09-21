#ifndef DARKPHOTONCOMPLETEANALYSIS_HH
#define DARKPHOTONCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonCompleteAnalysis : public FullAnalysis
{
    public:
        DarkPhotonCompleteAnalysis();

    private:
        void addSingleProcess(std::shared_ptr<Process> process, std::string filePathway,
                              std::string fileName, std::string crossSectionName,
                              std::shared_ptr<CrossSectionReader> crossReader,
                              std::shared_ptr<HistNameFinder> finder, bool isData = false);
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif