#include "CMSAnalysis/Modules/interface/DummyScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>


DummyScaleFactor::DummyScaleFactor() : ElectronScaleFactor("")
{
	
}

ParticleCollection<Particle> DummyScaleFactor::getParticles(const EventInput* input) const
{
    return {std::vector<Particle>()};
}

void DummyScaleFactor::loadScaleFactors(Json::Value output)
{
    // Do nothing
}
