#ifndef LEPTON_HH
#define LEPTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/RecoParticle.hh"


class LeptonJet;

//Lepton
class Lepton : public RecoParticle
{
    public:
        using RecoParticle::RecoParticle;

        Lepton(const Particle &particle);

        double getIsolation() const;
        bool isIsolated() const {throw std::runtime_error("error: Lepton::isIsolated() not implemented");};
        
        //Identifies Selection Fit Level [Tight,Medium,Loose]
        bool isTight() const;
        bool isMedium() const;
        bool isLoose() const;
};
#endif