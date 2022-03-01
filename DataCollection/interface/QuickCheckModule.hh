#ifndef QUICKCHECKMODULE_HH
#define QUICKCHECKMODULE_HH

#include "AnalysisModule.hh"

// This is meant to be a quick and dirty count
class QuickCheckModule : public AnalysisModule
{
    public:
        virtual bool process() override;
        virtual void finalize() override;

    private:
        int totalEvents = 0;
        int fourElectrons = 0;
        int fourMuons = 0;
        int twoOfEach = 0;

        int fourElectronPastCuts = 0;
        int fourMuonPastCuts = 0;
        int twoOfEachPastCuts = 0;

};

#endif