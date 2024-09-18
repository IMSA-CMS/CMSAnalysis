#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"

#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"

void MLTest(const char* sgFile, const char* bgFile, double stripVal) {
    // Open signal and background files
    TFile* inputFile = TFile::Open(sgFile);
    if (!inputFile) {
        std::cerr << "ERROR: Could not open signal file " << sgFile << std::endl;
        exit(1);
    }
    TTree* signalTree = (TTree*)inputFile->Get("Signal");

    TFile* bgInputFile = TFile::Open(bgFile);
    if (!bgInputFile) {
        std::cerr << "ERROR: Could not open background file " << bgFile << std::endl;
        exit(1);
    }
    TTree* backgroundTree = (TTree*)bgInputFile->Get("Signal");

    // Load the trained model
    TMVA::Reader* reader = new TMVA::Reader("!Color:!Silent");

    
    Float_t nParticles, sumPt, deltaPt, deltaR, eta, maxIsolation, leadingPt;

    // Initialize LeptonJetMLStripModule and add variables
    LeptonJetMLStripModule leptonMod;
    leptonMod.initialize();

    //////////////////// OLD BDT ///////////////////////

    // reader->AddVariable("deltaPt", &deltaPt);
    // reader->AddVariable("deltaR", &deltaR);
    // reader->AddVariable("eta", &eta);
    // reader->AddVariable("maxIsolation", &maxIsolation);
    // reader->AddVariable("nParticles", &nParticles);
    // reader->AddVariable("sumPt", &sumPt);

    // // Book the BDT method
    // TString methodName = "BDT method";
    // TString weightFile = "dataset/weights/TMVAClassification_BDT.weights.xml";
    // reader->BookMVA(methodName, weightFile);

    // signalTree->SetBranchAddress("deltaPt", &deltaPt);
    // signalTree->SetBranchAddress("deltaR", &deltaR);
    // signalTree->SetBranchAddress("eta", &eta);
    // signalTree->SetBranchAddress("maxIsolation", &maxIsolation);
    // signalTree->SetBranchAddress("nParticles", &nParticles);
    // signalTree->SetBranchAddress("sumPt", &sumPt);

    // // Loop over background events
    // backgroundTree->SetBranchAddress("deltaPt", &deltaPt);
    // backgroundTree->SetBranchAddress("deltaR", &deltaR);
    // backgroundTree->SetBranchAddress("eta", &eta);
    // backgroundTree->SetBranchAddress("maxIsolation", &maxIsolation);
    // backgroundTree->SetBranchAddress("nParticles", &nParticles);
    // backgroundTree->SetBranchAddress("sumPt", &sumPt);

    //////////////////// NEW BDT ////////////////////////

    reader->AddVariable("deltaR", &deltaR);
    reader->AddVariable("eta", &eta);
    reader->AddVariable("leadingPt", &leadingPt);
    reader->AddVariable("nParticles", &nParticles);

    // Book the BDT method
    TString methodName = "BDT method";
    TString weightFile = "dataset/weights/TMVAClassification_BDT.weights.xml";
    reader->BookMVA(methodName, weightFile);

    signalTree->SetBranchAddress("leadingPt", &leadingPt);
    signalTree->SetBranchAddress("deltaR", &deltaR);
    signalTree->SetBranchAddress("eta", &eta);
    signalTree->SetBranchAddress("nParticles", &nParticles);

    // Loop over background events
    backgroundTree->SetBranchAddress("leadingPt", &leadingPt);
    backgroundTree->SetBranchAddress("deltaR", &deltaR);
    backgroundTree->SetBranchAddress("eta", &eta);
    backgroundTree->SetBranchAddress("nParticles", &nParticles);

    ///////////////////// END BDT ///////////////////////////////

    // Evaluate the BDT response for the signal events
    double nSignal = signalTree->GetEntries();
    double nBackground = backgroundTree->GetEntries();
    double nSignalPassed = 0;
    double nBackgroundPassed = 0;

    for (int i = 0; i < nSignal; ++i) {
        signalTree->GetEntry(i);

        //std::cout << "DELTA" << deltaPt << std::endl;

        Double_t response = reader->EvaluateMVA(methodName);
        //std::cout << response << std::endl;
        if (response > stripVal) { // Assuming a cut value of 0.16, adjust as needed
            nSignalPassed++;
        }
    }

    // Evaluate the BDT response for the background events
    for (int i = 0; i < nBackground; ++i) {
        backgroundTree->GetEntry(i);
        Double_t response = reader->EvaluateMVA(methodName);
        if (response < stripVal) { // Assuming a cut value of 0.16, adjust as needed
            nBackgroundPassed++;
        }
    }

    // Calculate efficiencies
    double signalEfficiency = nSignalPassed / nSignal;
    double backgroundEfficiency = nBackgroundPassed / nBackground;

    // Output the results
    std::cout << "Signal Efficiency: " << signalEfficiency << std::endl;
    std::cout << "Background Rejection: " << backgroundEfficiency << std::endl;

    // Clean up
    delete reader;
    inputFile->Close();
    bgInputFile->Close();
}