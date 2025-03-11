#ifndef MULTIYEARSCALEFACTOR_HH
#define MULTIYEARSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

#include <map>
#include <string>
#include <memory>

class EventInput;
class MuonScaleFactor;
class ElectronScaleFactor;


class MultiYearScaleFactor : public ScaleFactor 
{
public:
	void addMuonScaleFactor(std::string year, std::shared_ptr<MuonScaleFactor> scaleFactor);
	void addElectronScaleFactor(std::string year, std::shared_ptr<ElectronScaleFactor> scaleFactor);

    virtual double getScaleFactor(const EventInput* input) const override;
private:
    std::map<std::string, std::shared_ptr<MuonScaleFactor>> muonScaleFactors;
    std::map<std::string, std::shared_ptr<ElectronScaleFactor>> electronScaleFactors;

};

#endif // MULTIYEARSCALEFACTOR_HH
