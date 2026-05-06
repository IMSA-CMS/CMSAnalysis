#include "CMSAnalysis/Modules/interface/L1PrefiringWeightRecipe.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

L1PrefiringWeightRecipe::L1PrefiringWeightRecipe(std::string name, std::shared_ptr<ScaleFactorReader> reader)
    : EventScaleFactor(name, reader)
{
    
}

std::vector<std::string> L1PrefiringWeightRecipe::getKey(const EventInput* input) const
{
    if (input == nullptr) 
    {
        std::cout << "EventInput is nullptr" << std::endl;
    }
    
    double nominalWeight = getNominalWeight(input);
    return std::vector<std::string>{std::to_string(nominalWeight)};
}

double L1PrefiringWeightRecipe::getNominalWeight(const EventInput* input) const
{
    if (input == nullptr) 
    {
        std::cout << "EventInput is nullptr" << std::endl;
        return 1.0;
    }
    
    try 
    {
        ParticleCollection<Particle> weights = input->getSpecial("L1PreFiringWeight_Nom");
        if (weights.size() > 0) 
        {
            return weights[0].getPt();
        }
    } 
    catch (...) {}
    
    std::cout << "L1PreFiringWeight_Nom not found" << std::endl;
    return 1.0;
}

double L1PrefiringWeightRecipe::getUpWeight(const EventInput* input) const
{
    if (input == nullptr) 
    {
        std::cout << "EventInput is nullptr" << std::endl;
        return 1.0;
    }
    
    try 
    {
        ParticleCollection<Particle> weights = input->getSpecial("L1PreFiringWeight_Up");
        if (weights.size() > 0)
		{
			return weights[0].getPt();
		} 
    } 
    catch (...) {}
    
    std::cout << "L1PreFiringWeight_Up not found" << std::endl;
    return 1.0;
}

double L1PrefiringWeightRecipe::getDownWeight(const EventInput* input) const
{
    if (input == nullptr) 
    {
        std::cout << "EventInput is nullptr" << std::endl;
        return 1.0;
    }
    
    try 
    {
        ParticleCollection<Particle> weights = input->getSpecial("L1PreFiringWeight_Dn");
        if (weights.size() > 0) 
		{
			return weights[0].getPt();
    	} 
	}
    catch (...) {}
    
    std::cout << "L1PreFiringWeight_Dn not found" << std::endl;
    return 1.0;
}