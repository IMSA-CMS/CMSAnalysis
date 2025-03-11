// #ifndef HIGGSCHANNELSANALYSIS_HH
// #define HIGGSCHANNELSANALYSIS_HH

// #include "FullAnalysis.hh"
// #include "Channel.hh"
// #include <memory>
// #include <vector>

// class HiggsChannelsAnalysis : public FullAnalysis
// {
//     public:
//         HiggsChannelsAnalysis();
//         std::vector<std::string> makeChannels(std::vector<std::string> particles, int length, bool checkPairs);
//         TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false, std::string channelName = "") const override;

//     private:
//         std::vector<std::shared_ptr<Channel>> channels;
// };

// #endif