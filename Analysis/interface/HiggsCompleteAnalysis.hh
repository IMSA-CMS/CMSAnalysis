#ifndef HIGGSCOMPLETEANALYSIS_HH
#define HIGGSCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include <vector>

const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu",
                                            "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut",
                                            "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};

const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu",
                                          "uue",  "uuu",  "ee",   "e e",  "eu",   "e u",  "uu",  "u u", "none"};

class HiggsCompleteAnalysis : public FullAnalysis
{
  public:
    HiggsCompleteAnalysis();
    double getBranchingRatio(const std::string &channel);
    // bool checkChannelName(std::string channelName, double massTarget);

    // TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false,
    // std::string channelName = "") const override;
};

#endif