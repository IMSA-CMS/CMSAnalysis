#include "TFile.h"
#include "TMVA/Reader.h"

void ReadJets(Jets jetInputs)
{
    TMVA::Reader reader {"!Color:!Silent:!V"};
    vector realJets = new vector<Jet>();
    for (Jet j : jetInputs)
    {

        reader.AddVariable("pt", j.GetPt());
        reader.AddVariable("nParticles", &nParticles);
        reader.AddVariable("phi", &phi);
        reader.AddVariable("eta", &eta);
        reader.AddVariable("mass", &mass);
        reader.AddVariable("deltaR", &deltaR);
        reader.BookMVA("MLP", "dataset/weights/TMVAClassification_MLP.weights.xml");
        double classification = reader.EvaluateMVA("MLP", jetInputs); //not sure about this line
        if (classification >= 0.5)
        {
            realJets.push_back(j);
        }
    }
}