#include "CMSAnalysis/Filters/interface/HiggsCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"


bool HiggsCut::checkEventInternal(const Event& event) const
{
    const auto particles = event.getParticles();
    //if(particles.calculateSameSignInvariantMass() < 800) {
    //    return false;
    //}
    if(particles.getLeadingPt() < 5) {
    	return false;
    }

    // for(auto particle : particles) {
    //     if(particle.getType() == ParticleType::electron()) 
    //    	{
    //        std::cout << "electron kept" << std::endl;
    //     }
    // }
    return true;
}
