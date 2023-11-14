#ifndef INVARIANTMASSFILTER_HH
#define INVARIANTMASSFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class InvariantMassFilter : public Filter
{
    public:
        InvariantMassFilter(double cut, EventInput::RecoLevel inputLvl = EventInput::RecoLevel::Reco);
    protected:
        virtual std::string getFilterString(const EventInput* inputMod) const override;

    private:
        // Checks to see if opposite-sign invariant mass is vetoed
        bool checkInvariantMass(Particle p1, Particle p2);

        // Checks to see if particles are the same lepton flavor
        bool checkFlavor(Particle p1, Particle p2);

        // Checks to see if the particles are opposite sign
        bool checkSign(Particle p1, Particle p2);

        double cut;
        EventInput::RecoLevel inputLevel;
};

#endif