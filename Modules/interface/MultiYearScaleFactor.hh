#ifndef MULTIYEARSCALEFACTOR_HH
#define MULTIYEARSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

#include <map>
#include <string>
#include <memory>

class EventInput;
class MuonScaleFactor;
class ElectronScaleFactor;

template <typename T>
class MultiYearScaleFactor : public T 
{
public:
	//void addScaleFactor(std::string year, std::shared_ptr<ScaleFactor> scaleFactor);
	//void addElectronScaleFactor(std::string year, std::shared_ptr<ElectronScaleFactor> scaleFactor);
    MultiYearScaleFactor(std::string name, std::shared_ptr<MultiYearScaleFactorReader>) : ScaleFactor(name) {}
    //virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
	    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;

protected:
        virtual std::string getKey(const EventInput* input) const override;
// private:
//     std::map<std::string, std::shared_ptr<ScaleFactor>> scaleFactors;
//     // std::map<std::string, std::shared_ptr<ElectronScaleFactor>> electronScaleFactors;

};
template <typename T> 
std::string MultiYearScaleFactor<T>::getKey(const EventInput* input) const
{
	auto year = input->getFileParams()->getParameters().find("Year");
	if (year == input->getFileParams()->getParameters().end())
	{
		throw std::runtime_error ("Year not found in file parameters");
	}
	return year->second + T::getKey(input);
}

#endif // MULTIYEARSCALEFACTOR_HH
