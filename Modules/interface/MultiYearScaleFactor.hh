#ifndef MULTIYEARSCALEFACTOR_HH
#define MULTIYEARSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/src/FileParams.cc"
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <iostream>
#include <stdexcept>
class MuonScaleFactor;
class ElectronScaleFactor;
class MultiYearScaleFactorReader;

template <typename T>
class MultiYearScaleFactor : public T 
{
public:
	//void addScaleFactor(std::string year, std::shared_ptr<ScaleFactor> scaleFactor);
	//void addElectronScaleFactor(std::string year, std::shared_ptr<ElectronScaleFactor> scaleFactor);
    MultiYearScaleFactor(std::string name, std::shared_ptr<MultiYearScaleFactorReader>) : ScaleFactor(name) {}
	MultiYearScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader, ParticleType particleType) : T(name, reader, particleType) {}
    //virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
    virtual double getScaleFactor(const EventInput* input, ScaleFactor::SystematicType type = ScaleFactor::SystematicType::Nominal) const;

protected:
		virtual std::vector<std::string> getKey(const EventInput* input) const override;

};


template <typename T> 
double MultiYearScaleFactor<T>::getScaleFactor(const EventInput* input, ScaleFactor::SystematicType type) const
{
    // Delegate to the base class T's implementation
    return T::getScaleFactor(input, type);
}

template <typename T> 
std::vector<std::string> MultiYearScaleFactor<T>::getKey(const EventInput* input) const
{
    auto year = input->getFileParams()->getParameters().find("Year");
    if (year == input->getFileParams()->getParameters().end())
    {
        throw std::runtime_error ("Year not found in file parameters");
    }
    
    std::vector<std::string> baseKeys = T::getKey(input);
    for (auto& key : baseKeys) 
	{
		key = year->second + "_" + key; // Prepend the year to each base key
	}
    // std::vector<std::string> result;
    // result.reserve(baseKeys.size() + 1);
    // result.push_back(year->second);
    
    // for (const auto& key : baseKeys) {
    //     result.push_back(key);
    // }
	
	// std::cout << "Base keys:\n";
	// for (const auto& key : baseKeys) {
	// 	std::cout << key << " ";
	// }
	// std::cout << std::endl;
	// std::cout << std::endl;

    return baseKeys;
}
#endif // MULTIYEARSCALEFACTOR_HH
