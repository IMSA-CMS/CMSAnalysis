#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
 
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
 
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

//To predict if a lepton jet is real, I am using a combination of CUDA-accelerated
//and Multi-core accelerated DNN's. I am using all of the data reduction methods.
//The work left to do is to replace the training dataset used with the generated
//dataset and change the names to build without errors. (10/24/2021)

bool returnState(TString& myMethodList)
{        

    bool isReal;
//    TTree trainTree = DataStripModule :: tree;
//    TTree testTree = DataStripModule :: tree2;


    TMVA::Tools::Instance();


    std::map<std::string,int> Use;
 
    // Cut optimisation
    Use["Cuts"]            = 1;
    Use["CutsD"]           = 0;
    Use["CutsPCA"]         = 0;
    Use["CutsGA"]          = 0;
    Use["CutsSA"]          = 0;

    // Neural Networks (all are feed-forward Multilayer Perceptrons)
    Use["MLP"]             = 1; // Recommended ANN
    Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
    Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
    Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
    Use["TMlpANN"]         = 0; // ROOT's own ANN
    #ifdef R__HAS_TMVAGPU
    Use["DNN_GPU"]         = 0; // CUDA-accelerated DNN training.
    #else
    Use["DNN_GPU"]         = 0;
    #endif
    
    #ifdef R__HAS_TMVACPU
    Use["DNN_CPU"]         = 0; // Multi-core accelerated DNN.
    #else
    Use["DNN_CPU"]         = 0;
    #endif


    std::cout << std::endl;
    std::cout << "==> Start TMVAClassification" << std::endl;
    
    // Select methods (don't look at this code - not of interest)
    if (myMethodList != "") {
        for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
    
        std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
        for (UInt_t i=0; i<mlist.size(); i++) {
            std::string regMethod(mlist[i]);
    
            if (Use.find(regMethod) == Use.end()) {
                std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
                for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
                std::cout << std::endl;
                return 1;
            }
            Use[regMethod] = 1;
        }
    }

    // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
    TFile *input(0);
    TString fname = "./tmva_class_example.root";
    if (!gSystem->AccessPathName( fname )) {
        input = TFile::Open( fname ); // check if file in local directory exists
    }
    else {
        TFile::SetCacheFileDir(".");
        input = TFile::Open("leptonJetData.root", "CACHEREAD");
    }
    if (!input) {
        std::cout << "ERROR: could not open data file" << std::endl;
        exit(1);
    }
    std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

    
    // Register the training and test trees
    
    TTree *signalTree     = (TTree*)input->Get("DataStripTree");
    TTree *background     = (TTree*)input->Get("DataStripTree2");
    
    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
    TString outfileName( "TMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );


    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                                "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    
    TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");
    
    //change var1 to index and var2 to pt
    //dataloader->AddVariable("jetIndex", "Jet Index", "", 'F');
    dataloader->AddVariable("pt", "Transverse Momentum", "", 'F');
    dataloader->AddVariable("nParticles", "Number of Particles", "", 'F');
    dataloader->AddVariable("phi", "Pseudorapidity", "", 'F');
    dataloader->AddVariable("eta", "Azimuthal Angle", "", 'F');
    dataloader->AddVariable("mass", "Mass", "", 'F');
    
    
    dataloader->AddSpectator( "spec1 := jetIndex*2",  "Spectator 1", "units", 'F' );
    
    
    // global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    
    // You can add an arbitrary number of signal or background trees
    dataloader->AddSignalTree    ( signalTree,     signalWeight );
    dataloader->AddBackgroundTree( background, backgroundWeight );



    // I am using all of the data reduction methods
    if (Use["Cuts"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
    
    if (Use["CutsD"])
        factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsD",
                            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );
    
    if (Use["CutsPCA"])
        factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsPCA",
                            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
    
    if (Use["CutsGA"])
        factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsGA",
                            "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
    
    if (Use["CutsSA"])
        factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsSA",
                            "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
    
    if (Use["MLP"])
        factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

    //Using both CUDA-Accelerated and Multi-core Accelerated Deep Neural Networks
    if (Use["DNN_CPU"] or Use["DNN_GPU"]) {
    // General layout.

        TString layoutString ("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

        // Define Training strategy. One could define multiple strategy string separated by the "|" delimiter



        TString trainingStrategyString = ("TrainingStrategy=LearningRate=1e-2,Momentum=0.9,"
                                            "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
                                            "WeightDecay=1e-4,Regularization=None,"
                                            "DropConfig=0.0+0.5+0.5+0.5");

        // General Options.
        TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                            "WeightInitialization=XAVIERUNIFORM");
        dnnOptions.Append (":"); dnnOptions.Append (layoutString);
        dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);


        // Cuda implementation.
        if (Use["DNN_GPU"]) {
            TString gpuOptions = dnnOptions + ":Architecture=GPU";
            factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_GPU", gpuOptions);

        }
        // Multi-core CPU implementation.
        if (Use["DNN_CPU"]) {
            TString cpuOptions = dnnOptions + ":Architecture=CPU";
            factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);
        }
        cout << "thisLine" << __LINE__ <<" Works\n";

    }

    // Train MVAs using the set of training events
    factory->TrainAllMethods();
    cout << "thisLine" << __LINE__ <<" Works\n";

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();
    cout << "thisLine" << __LINE__ <<" Works\n";

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    cout << "thisLine" << __LINE__ <<" Works\n";

    // Save the output
    outputFile->Close();
    
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    
    delete factory;
    delete dataloader;
    cout << "thisLine" << __LINE__ <<" Works\n";

    // Launch the GUI for the root macros
    if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );
    cout << "thisLine" << __LINE__ <<" Works\n";
return false;
}

void PredictJet()//int argc, char** argv)
{
// Select methods (don't look at this code - not of interest)
TString methodList;
/*
for (int i=1; i<argc; i++) {
    TString regMethod(argv[i]);
    if(regMethod=="-b" || regMethod=="--batch") continue;
    if (!methodList.IsNull()) methodList += TString(",");
    methodList += regMethod;
}
*/
returnState(methodList);
}