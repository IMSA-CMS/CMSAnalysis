#ifndef FILESTRIPMODULE_HH
#define FILESTRIPMODULE_HH

#include "AnalysisModule.hh"
#include "TTree.h"
#include <vector>

// A class that can strip a file down to only the needed data, and stores the data in a TTree.
// To use file stripping, call its plan through runAnalyzer, and the file name of the stripped file
// can be changed in FileStripPlan.cc. 
class FileStripModule : public AnalysisModule
{
    public:
    // Called in FileStripPlan.hh. The name of the stripped data file is specified in the name 
    //parameter of this constructor. The output file of runAnalyzer only contains nEvents.
	FileStripModule(std::string name);
    virtual void finalize() override;
    // Checks if a GenSimParticle is inside of the collection given
    int checkGenSim(const GenSimParticle &genSim, ParticleCollection<Particle> &collection);
    virtual void initialize() override;

    protected:
    bool process() override;

    private:
    std::string name;
    TFile* file;
    TTree* tree;

	UInt_t elecSize;
	std::vector<float> elecEta;
	std::vector<float> elecPhi;
    std::vector<float> elecMass;
    std::vector<int> elecCharge;
    std::vector<float> elecPt;
    std::vector<float> elecReliso;

    UInt_t muonSize;
    std::vector<float> muonPhi;
    std::vector<float> muonEta;
    std::vector<float> muonMass;
    std::vector<int> muonCharge;
    std::vector<float> muonPt;
    std::vector<float> muonReliso;

    std::vector<float> metPhi;
    std::vector<float> metPt;

    UInt_t jetSize;
    std::vector<float> jetEta;
    std::vector<float> jetPhi;
    std::vector<float> jetMass;
    std::vector<float> jetPt;

    UInt_t genSize;
    std::vector<int> genPID;
    std::vector<int> genStatus;
    std::vector<float> genEta;
    std::vector<float> genPhi;
    std::vector<float> genMass;
    std::vector<float> genPt;
    std::vector<int> genD1;
    std::vector<int> genD2;
    std::vector<int> genM1;
    std::vector<int> genM2;

    std::vector<int> elecIDPass;
    std::vector<int> muonIDPass;
};

#endif
