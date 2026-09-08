#ifndef UNCERTAINTY_HH
#define UNCERTAINTY_HH

#include "CMSAnalysis/Modules/interface/SystematicEventInput.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

template <typename T>
class Uncertainty
{
public:
	Uncertainty(const EventInterface **eventInterface);

	// Accessors for the Up and Down inputs
	SystematicEventInput *getUpInput() const { return upInput.get(); }
	SystematicEventInput *getDownInput() const { return downInput.get(); }

private:
	std::unique_ptr<SystematicEventInput> upInput;
	std::unique_ptr<SystematicEventInput> downInput;
};

template <typename T>
inline Uncertainty::Uncertainty(const EventInterface **eventInterface)
{
	upInput = std::make_unique<T>(eventInterface, ScaleFactor::SystematicType::Up);
	downInput = std::make_unique<T>(eventInterface, ScaleFactor::SystematicType::Down);
}

#endif