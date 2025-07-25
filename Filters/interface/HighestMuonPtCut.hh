#ifndef HIGHESTMUONPTCUT_HH
#define HIGHESTMUONPTCUT_HH

#include "Cut.hh"

class HighestMuonPtCut : public Cut
{
    public:
        HighestMuonPtCut(double iCutValue): cutValue(iCutValue){}
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
    private:
        double cutValue;
};  

#endif