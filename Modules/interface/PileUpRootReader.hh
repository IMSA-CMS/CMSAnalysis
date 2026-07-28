#ifndef PILEUPROOTREADER_HH
#define PILEUPROOTREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <map>
#include <string>

class EventInput;
class TH1;

class PileUpRootReader : public ScaleFactorReader 
{
public:

    PileUpRootReader(std::string datafilename, std::string mcfilename, std::string datahistname,
         std::string mchistname, std::string datahistname_up, std::string datahistname_down);
    // virtual double getScaleFactor(const EventInput* input) const override;
    virtual std::map<std::string, ScaleFactor::ScaleFactorSet> readData() override;
    void printScaleFactors() const;  

private:
	TH1* histogram;
    TH1* up;
    TH1* down;
};

#endif // PileUpRootReader_HH
