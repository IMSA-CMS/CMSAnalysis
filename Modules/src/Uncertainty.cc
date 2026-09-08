#include "CMSAnalysis/Modules/interface/Uncertainty.hh"

// Constructor: create Up and Down SystematicEventInput objects
Uncertainty::Uncertainty(const EventInterface **eventInterface, )
{
	upInput = std::make_unique<SystematicEventInput>(eventInterface, ScaleFactor::SystematicType::Up);
	downInput = std::make_unique<SystematicEventInput>(eventInterface, ScaleFactor::SystematicType::Down);
}