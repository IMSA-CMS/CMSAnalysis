#include "CMSAnalysis/Filters/interface/HiggsCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

double HiggsCut::electronThreeChannelCut = 5;
double HiggsCut::muonThreeChannelCut = 5;

bool HiggsCut::checkEventInternal(const Event& event) const
{
    const auto particles = event.getParticles();


    int numLeptons = particles.getLeptonTypeCount(ParticleType::electron()) + particles.getLeptonTypeCount(ParticleType::muon());
    double leptonPt; 
    ParticleType leptonType;

    // Finds the third lepton pt
    if(particles.getLeadingPt() > 5 && numLeptons == 3) 
    {
        // for (auto particle : particles)
        // {
        //     std::cout << particle << '\n';
        // }

        if (particles.getNumPosParticles() == 2)
        {
            auto particle = particles.getNegParticles()[0];
            leptonPt = particle.getPt();
            leptonType = particle.getType();
        }
        else if (particles.getNumNegParticles() == 2)
        {
            auto particle = particles.getPosParticles()[0];
            leptonPt = particle.getPt();
            leptonType = particle.getType();
        }
        else
        {
            auto particlePair = particles.chooseParticles();
            for (auto particle : particles)
            {
                if (particle != particlePair.first && particle != particlePair.second)
                {
                    leptonPt = particle.getPt();
                    leptonType = particle.getType();
                }
            }
        }

        if (leptonType == ParticleType::electron() && leptonPt > electronThreeChannelCut)
        {
            std::cout << true << '\n';
            return true;
        }
        else if (leptonType == ParticleType::muon() && leptonPt > muonThreeChannelCut)
        {
            std::cout << true << '\n';
            return true;
        }
    }

    std::cout << false << '\n';
    return false;
}
