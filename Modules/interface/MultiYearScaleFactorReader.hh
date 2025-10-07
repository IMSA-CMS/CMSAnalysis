#ifndef MULTIYEARSCALEFACTORREADER_HH
#define MULTIYEARSCALEFACTORREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include <map>
#include <string>
#include <memory>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class MultiYearScaleFactorReader : public ScaleFactorReader
{
public:
    MultiYearScaleFactorReader() = default;
    virtual ~MultiYearScaleFactorReader() = default;

    void addYearData(const std::string& year, std::shared_ptr<ScaleFactorReader> reader);

    virtual std::map<std::string, ScaleFactor::ScaleFactorSet> readData() override;
private:
    std::map<std::string, std::shared_ptr<ScaleFactorReader>> yearData;
};

#endif