#ifndef SAMESIGNINVARIANTMASSFILTER_HH
#define SAMESIGNINVARIANTMASSFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

class SameSignInvariantMassFilter : public Filter
{
    public:
        SameSignInvariantMassFilter(double cut);
    protected:
        virtual std::string getFilterString(const EventInput* inputMod) const override;

    private:
        // Checks to see if opposite-sign invariant mass is vetoed
        bool checkSameSignInvariantMass(Particle p1, Particle p2);

        // Checks to see if particles are the same lepton flavor
        bool checkFlavor(Particle p1, Particle p2);

        // Checks to see if the particles are opposite sign
        bool checkSign(Particle p1, Particle p2);

        double cut;
};

#endif