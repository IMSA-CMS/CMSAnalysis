#ifndef FULLANALYSIS_HH
#define FULLANALYSIS_HH

#include "Channel.hh"
#include "Correction.hh"
#include "CrossSectionReader.hh"
#include "MultiSystematic.hh"
#include "SimpleEstimator.hh"
#include "SingleProcess.hh"
#include "TH1.h"
#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

class HistNameFinder;

class FullAnalysis
{
  public:
    FullAnalysis(double luminosity) : luminosity(luminosity)
    {
        TH1::SetDefaultSumw2();
    }

    virtual ~FullAnalysis() {}

    // To my knowledge, this has to be defined in derived classes to use that class' variables
    std::shared_ptr<Channel> getChannel(std::string name);
    std::vector<std::shared_ptr<Channel>> getChannels() const { return channels; }
    double getLuminosity() const { return luminosity; }

    std::vector<TH1 *> getHistograms(const HistVariable &histType, Channel::Label label, const std::string &channel,
                                     bool scaleToExpected = false);
    TH1 *getHist(HistVariable histType, std::string processName, bool scaleToExpected = false,
                 std::string channelName = "") const;

    virtual std::vector<std::string> getSystematics() const { return {}; };

  protected:
    std::vector<std::shared_ptr<Channel>>& getChannelsProtected() { return channels; }

  private:
    std::vector<std::shared_ptr<Channel>> channels;
    double luminosity;
};

#endif
