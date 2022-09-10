#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "TFile.h"
#include "TMVA/Reader.h"

vector<LeptonJet> ReadJet()
{
    //TODO: make a way of reading jets, read them into jetInputs from a file
    vector<LeptonJet>* jetInputs = new vector<LeptonJet>();

    //This strips jetInputs of any fake jets using the MLP
    vector<LeptonJet>* realJets = new vector<LeptonJet>();

    TMVA::Reader reader {"!Color:!Silent:!V"};
    float pt, phi, eta, mass, deltaR, nParticles;
    int jetIndex;

    //This adds the 6 variables and uses them as the inputs
    reader.AddVariable("pt", &pt);
    reader.AddVariable("nParticles", &nParticles);
    reader.AddVariable("phi", &phi);
    reader.AddVariable("eta", &eta);
    reader.AddVariable("mass", &mass);
    reader.AddVariable("deltaR", &deltaR);

    reader.AddSpectator( "spec1 := jetIndex*2", &jetIndex);
    reader.BookMVA("MLP", "dataset/weights/TMVAClassification_MLP.weights.xml");

    //loops through and adds
    jetIndex = 0;
    for (LeptonJet j : *jetInputs)
    {
        pt = j.getFourVector().Pt();
        nParticles = j.getNumParticles();
        phi = j.getFourVector().Phi();
        eta = j.getFourVector().Eta();
        mass = j.getFourVector().mass();double deltaR = 0;
        for (Particle p : j.getParticles())
        {
            for (Particle q : j.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
            }
        }

        double classification = reader.EvaluateMVA("MLP");
        if (classification >= 0.5)
        {
            realJets->push_back(j);
        }
        jetIndex++;
    }

    //This is a sample that shows how to classify one jet
    pt = 8000; nParticles = 800; phi = 0.1; eta = 3.04; mass = 0.05; deltaR = 110.16;
    if (reader.EvaluateMVA("MLP") >= 0.5)
    {
        cout << "This jet is real\n";
    }
    else
    {
        cout << "You a fake one\n";
    }

    //returns only the real jets
    return *realJets;
}