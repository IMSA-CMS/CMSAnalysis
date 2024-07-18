#include "CMSAnalysis/Filters/interface/HiggsCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

double HiggsCut::electronThreeChannelCut = 250;
double HiggsCut::muonThreeChannelCut = 250;

bool HiggsCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const auto particles = event.getParticles(EventInput::RecoLevel::Reco);

    int numLeptons = particles.getLeptonTypeCount(ParticleType::electron()) + particles.getLeptonTypeCount(ParticleType::muon());
    // double leptonPt = 0; 
    ParticleType leptonType;

    if (numLeptons == 4 || numLeptons == 2)
    {
        return true;
    }

    // Finds the third lepton pt
    if(numLeptons == 3) 
    {
        if (particles.getNumPosParticles() == 2)
        {
            auto particle = particles.getNegParticles()[0];
            // leptonPt = particle.getPt();
            leptonType = particle.getType();
        }
        else if (particles.getNumNegParticles() == 2)
        {
            auto particle = particles.getPosParticles()[0];
            // leptonPt = particle.getPt();
            leptonType = particle.getType();
        }
        else
        {
            auto particlePair = particles.chooseParticles(true);
            for (auto particle : particles)
            {
                if (particle != particlePair.first && particle != particlePair.second)
                {
                    // leptonPt = particle.getPt();
                    leptonType = particle.getType();
                }
            }
        }

        // if (leptonType == ParticleType::electron() && leptonPt > electronThreeChannelCut)
        // {
        //     return true;
        // }
        // else if (leptonType == ParticleType::muon() && leptonPt > muonThreeChannelCut)
        // {
        //     return true;
        // }

        return false;
        // return false;
    } 
    else
    {
        auto electrons = event.getElectrons();
        auto muons = event.getMuons();
        if (electrons.size() == 2 
        && muons.size() == 2
        && electrons[0].getCharge() == electrons[1].getCharge()
        && muons[0].getCharge() == muons[1].getCharge()
        && electrons[0].getCharge() != muons[0].getCharge()
        ) 
        {
            return true;
        }
        return false;
    }
}
