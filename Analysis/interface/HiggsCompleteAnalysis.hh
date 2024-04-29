#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsCompleteAnalysis : public FullAnalysis
{
    public:
        HiggsCompleteAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
        SingleProcess makeSingleProcess(std::vector<HistVariable> histVariables, std::vector<HistVariable> fitHistVariables, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections = {}) override;
        SingleProcess makeSignalProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections = {}) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() override {return channels;}
        TH1* getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false) const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
