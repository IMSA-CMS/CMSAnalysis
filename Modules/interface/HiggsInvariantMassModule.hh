#ifndef HIGGSINVARIANTMASSMODULE_HH
#define HIGGSINVARIANTMASSMODULE_HH

#include "AnalysisModule.hh"
#include "TTree.h"
#include <vector>
#include <string>

class HiggsInvariantMassModule : public AnalysisModule
{
    public: 
    HiggsInvariantMassModule(std::string name);
    virtual void initialize();
    virtual void finalize();

    protected: 
    bool process() override;

    private:
    std::string name;
    TFile* file;
    TTree* tree;

    UInt_t muonSize;
    std::vector<double> muonInvariantMass;
};

#endif