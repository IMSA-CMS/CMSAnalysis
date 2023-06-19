#ifndef LEPTON_HH
#define LEPTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"


class LeptonJet;

//Lepton
class Lepton : public Particle
{
    public:
        using Particle::Particle;

        Lepton(const Particle &particle);

        double getIsolation() const;
        bool isIsolated() const {throw std::runtime_error("error: Lepton::isIsolated() not implemented");};
        
        //Identifies Selection Fit Level [Tight,Medium,Loose]
        bool isTight() const;
        bool isMedium() const;
        bool isLoose() const;
};
#endif