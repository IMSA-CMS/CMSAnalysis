#ifndef MULTISCALEFACTOR_HH
#define MULTISCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

#include <map>
#include <string>
#include <memory>

class EventInput;
class MuonScaleFactor;
class ElectronScaleFactor;

class MultiScaleFactor : public ScaleFactor
{
public:
	//void addScaleFactor(std::string year, std::shared_ptr<ScaleFactor> scaleFactor);
	//void addElectronScaleFactor(std::string year, std::shared_ptr<ElectronScaleFactor> scaleFactor);
    MultiScaleFactor(std::string name, std::vector<std::shared_ptr<ScaleFactor>> scaleFactors) : ScaleFactor(name), scaleFactors(scaleFactors) {}
    //virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
	virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
protected:
        virtual std::string getKey(const EventInput* input) const override;
private:
	std::vector<std::shared_ptr<ScaleFactor>> scaleFactors;

	// If you want to use a map instead of a vector, uncomment the following lines:
	// std::map<std::string, std::shared_ptr<ScaleFactor>> scaleFactors;
//     std::map<std::string, std::shared_ptr<ScaleFactor>> scaleFactors;
//     // std::map<std::string, std::shared_ptr<ElectronScaleFactor>> electronScaleFactors;

};


#endif // MULTIYEARSCALEFACTOR_HH
