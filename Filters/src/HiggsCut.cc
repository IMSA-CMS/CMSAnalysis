#include "CMSAnalysis/Filters/interface/HiggsCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

double HiggsCut::electronThreeChannelCut = 250;
double HiggsCut::muonThreeChannelCut = 250;

bool HiggsCut::checkEventInternal(const Event& event, const EventInput* input) const
{
   
    const auto particles = event.getParticles(EventInput::RecoLevel::Reco);

    int numLeptons = particles.getLeptonTypeCount(ParticleType::electron()) +
                     particles.getLeptonTypeCount(ParticleType::muon()) +
                     particles.getLeptonTypeCount(ParticleType::tau());
     std::cout << "numLeptons = " << numLeptons << std::endl;
    //automatically rejects too little or too many
    if (numLeptons < 2 || numLeptons > 4)
    {
        return false;
    }
//count taus, if taus =0, then you need the total charge is 0, if number of taus is 1, still need the sum of the charges to not be 3 or -3. If there's more than one tau then it's all good. 
    double leptonPt = 0; 
    ParticleType leptonType;

    // tau logic for 4 lepton events
    if (numLeptons == 4)
    {
        int numTaus = particles.getLeptonTypeCount(ParticleType::tau());
        int totalCharge = 0; //makes sure that the total charge is zero if taus are zero
        for (const auto& particle : particles)
        {
            totalCharge += particle.getCharge();
        }

        if (numTaus == 0 && totalCharge != 0) return false;
        if (numTaus == 1 && (totalCharge == 3 || totalCharge == -3)) return false;
        // valid 4-lepton events continue to pt checks
    }

    // pT thresholds (2 and 4 lepton channels)
    if (numLeptons == 2 || numLeptons == 4)
    {
        for (auto particle : particles)
        {
            if (particle.getType() == ParticleType::electron() && particle.getPt() >= 40)
            {
                return true;
            }
            else if (particle.getType() == ParticleType::muon() && particle.getPt() >= 30)
            {
                return true;
            }
        }
        return false; 
    }

    // 3 lepton channel pT thresholds 
    if (numLeptons == 3) 
    {
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
            auto particlePair = particles.chooseParticles(true);
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
            return true;
        }
        else if (leptonType == ParticleType::muon() && leptonPt > muonThreeChannelCut)
        {
            return true;
        }

        return false;
    }
    return false;
}

