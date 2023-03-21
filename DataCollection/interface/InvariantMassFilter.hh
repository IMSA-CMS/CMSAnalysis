#ifndef INVARIANTMASSFILTER_HH
#define INVARIANTMASSFILTER_HH

#include "FilterModule.hh"
#include "Particle.hh"
#include "InputModule.hh"

class InvariantMassFilter : public FilterModule
{
    public:
        InvariantMassFilter(double cut, InputModule::RecoLevel inputLvl = InputModule::RecoLevel::Reco);
    protected:
        virtual std::string makeFilterString() override;

    private:
        // Checks to see if opposite-sign invariant mass is vetoed
        bool checkInvariantMass(Particle p1, Particle p2);

        // Checks to see if particles are the same lepton flavor
        bool checkFlavor(Particle p1, Particle p2);

        // Checks to see if the particles are opposite sign
        bool checkSign(Particle p1, Particle p2);

        double cut;
        InputModule::RecoLevel inputLevel;
};

#endif