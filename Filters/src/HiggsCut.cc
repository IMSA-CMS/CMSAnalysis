#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include <algorithm>

double HiggsCut::electronThreeChannelCut = 250;
double HiggsCut::muonThreeChannelCut = 250;

bool HiggsCut::checkEventInternal(const Event &event, const EventInput *input) const
{
    const auto particles = event.getParticles(EventInput::RecoLevel::Reco);

    const int numLeptons = particles.getLeptonTypeCount(ParticleType::electron()) +
                           particles.getLeptonTypeCount(ParticleType::muon()) +
                           particles.getLeptonTypeCount(ParticleType::tau());

    switch (numLeptons)
    {
    case 2:
        return std::ranges::any_of(particles, [](const auto &p) {
            return (p.getType() == ParticleType::electron() && p.getPt() >= 40) ||
                   (p.getType() == ParticleType::muon() && p.getPt() >= 30);
        });
    case 3: {
        // 3 lepton channel pT thresholds
        Particle lepton = Particle::nullParticle();

        if (particles.getNumPosParticles() == 2)
        {
            lepton = particles.getNegParticles()[0];
        }
        else if (particles.getNumNegParticles() == 2)
        {
            lepton = particles.getPosParticles()[0];
        }
        else
        {
            const auto particlePair = particles.chooseParticles(true);
            for (const auto &particle : particles)
            {
                if (particle != particlePair.first && particle != particlePair.second)
                {
                    lepton = particle;
                }
            }
        }

        if ((lepton.getType() == ParticleType::electron() && lepton.getPt() > electronThreeChannelCut) ||
            (lepton.getType() == ParticleType::muon() && lepton.getPt() > muonThreeChannelCut))
        {
            return true;
        }
        return false;
    }
    case 4: {
        int totalCharge = 0;
        for (const auto &particle : particles)
        {
            totalCharge += particle.getCharge();
        }

        if (totalCharge != 0)
        {
            return false;
        }
        return std::ranges::any_of(particles, [](const auto &p) {
            return (p.getType() == ParticleType::electron() && p.getPt() >= 40) ||
                   (p.getType() == ParticleType::muon() && p.getPt() >= 30);
        });
    }
    default:
        return false;
    }
}
