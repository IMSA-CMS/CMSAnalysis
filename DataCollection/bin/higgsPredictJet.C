#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <cmath>

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
#include "TMVA/IMethod.h"

// Essentially PredictJet.C but for Higgs events instead of dark photon events

bool returnState(TString &myMethodList)
{
	bool isReal;
	TMVA::Tools::Instance();
	std::map<std::string, int> Use;
	// Cut optimisation
	Use["Cuts"] = 1;
	Use["CutsD"] = 1;
	Use["CutsPCA"] = 1;
	Use["CutsGA"] = 1;
	Use["CutsSA"] = 1;
	// Neural Networks (all are feed-forward Multilayer Perceptrons)
	Use["MLP"] = 1;		 // Recommended ANN
	Use["MLPBFGS"] = 0;	 // Recommended ANN with optional training method
	Use["MLPBNN"] = 0;	 // Recommended ANN with BFGS training method and bayesian regulator
	Use["CFMlpANN"] = 0; // Depreciated ANN from ALEPH
	Use["TMlpANN"] = 0;	 // ROOT's own ANN
	// Boosted Decision Trees
	Use["BDT"] = 1;	 // Boosted Decision Trees (Adaptive Boost, recommended)
	Use["BDTG"] = 0; // uses Gradient Boost
	Use["BDTB"] = 0; // uses Bagging
	Use["BDTD"] = 0; // decorrelation + Adaptive Boost
	Use["BDTF"] = 0; // allow usage of fisher discriminant for node splitting
#ifdef R__HAS_TMVAGPU
	Use["DNN_GPU"] = 0; // CUDA-accelerated DNN training.
#else
	Use["DNN_GPU"] = 0;
#endif

#ifdef R__HAS_TMVACPU
	Use["DNN_CPU"] = 1; // Multi-core accelerated DNN.
#else
	Use["DNN_CPU"] = 1;
#endif
	std::cout << std::endl;
	std::cout << "==> Start TMVAClassification" << std::endl;
	// Select methods (don't look at this code - not of interest)
	if (myMethodList != "")
	{
		for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++)
			it->second = 0;

		std::vector<TString> mlist = TMVA::gTools().SplitString(myMethodList, ',');
		for (UInt_t i = 0; i < mlist.size(); i++)
		{
			std::string regMethod(mlist[i]);

			if (Use.find(regMethod) == Use.end())
			{
				std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
				for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++)
					std::cout << it->first << " ";
				std::cout << std::endl;
				return 1;
			}
			Use[regMethod] = 1;
		}
	}
	// input signal file here
	string sgFile = "higgs500.root";
	//sting sgFile = "newHiggs500.root";

	// input background files here
	string bgFiles[] =
		{
			//"dy10.root",
			//"dy50.root",
			"qcd500.root",
			"qcd700.root",
			"qcd1k.root",
			"qcd1_5k.root",
			"qcd2k.root"};

	TFile *input(0);
	TString fname = "./tmva_class_example.root";
	if (!gSystem->AccessPathName(fname))
	{
		input = TFile::Open(fname); // check if file in local directory exists
	}
	else
	{
		TFile::SetCacheFileDir(".");
		input = TFile::Open(sgFile.c_str(), "CACHEREAD"); //
	}
	if (!input)
	{
		std::cout << "ERROR: could not open data file" << std::endl;
		exit(1);
	}
	std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

	// Registers background files in background chain
	TTree *bg = new TTree();
	TChain *backgroundTree = new TChain("Signal"); // Signal of background files
	for (string file : bgFiles)
	{
		backgroundTree->Add(file.c_str());
	}

	// TFile* input = TFile::Open("qcd2kTrain.root");

	// Register the training and test trees

	TTree *signalTree = (TTree *)input->Get("Signal");
	// TTree *backgroundTree = (TTree *)input2->Get("Background");

	// Create a ROOT output file where TMVA will store ntuples, histograms, etc.
	TString outfileName("TMVA.root");
	TFile *outputFile = TFile::Open(outfileName, "RECREATE");

	TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile,
											   "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");

	TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

	// change var1 to index and var2 to pt
	// dataloader->AddVariable("jetIndex", "Jet Index", "", 'F');
	dataloader->AddVariable("pt1", "First Lepton Transverse Momentum", "", 'F');
	dataloader->AddVariable("pt2", "Second Lepton Transverse Momentum", "", 'F');
	dataloader->AddVariable("eta1", "First Lepton Pseudorapidity", "", 'F');
	dataloader->AddVariable("eta2", "Second Lepton Pseudorapidity", "", 'F');
	dataloader->AddVariable("phi1", "First Lepton Azimuthal Angle", "", 'F');
	dataloader->AddVariable("phi2", "Second Lepton Azimuthal Angle", "", 'F');
	// dataloader->AddVariable("mass", "Mass", "", 'F');
	dataloader->AddVariable("deltaR", "Jet Width", "", 'F');
	// dataloader->AddSpectator("spec1 := jetIndex*2", "Spectator 1", "units", 'F');

	// global event weights per tree (see below for setting event-wise weights)
	Double_t signalWeight = 1.0;
	Double_t backgroundWeight = 1.0;

	// You can add an arbitrary number of signal or background trees
	dataloader->AddSignalTree(signalTree, signalWeight);
	dataloader->AddBackgroundTree(backgroundTree, backgroundWeight);

	// dataloader->

	// I am using all of the data reduction methods
	if (Use["Cuts"])
		factory->BookMethod(dataloader, TMVA::Types::kCuts, "Cuts",
							"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart");

	if (Use["CutsD"])
		factory->BookMethod(dataloader, TMVA::Types::kCuts, "CutsD",
							"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate");

	if (Use["CutsPCA"])
		factory->BookMethod(dataloader, TMVA::Types::kCuts, "CutsPCA",
							"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA");

	if (Use["CutsGA"])
		factory->BookMethod(dataloader, TMVA::Types::kCuts, "CutsGA",
							"H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95");

	if (Use["CutsSA"])
		factory->BookMethod(dataloader, TMVA::Types::kCuts, "CutsSA",
							"!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale");

	if (Use["MLP"])
		factory->BookMethod(dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator");

	if (Use["BDT"])
		factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
							"!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");

	// Using both CUDA-Accelerated and Multi-core Accelerated Deep Neural Networks
	if (Use["DNN_CPU"] or Use["DNN_GPU"])
	{
		// General layout.

		TString layoutString("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

		// Define Training strategy. One could define multiple strategy string separated by the "|" delimiter

		TString trainingStrategyString = ("TrainingStrategy=LearningRate=1e-2,Momentum=0.9,"
										  "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
										  "WeightDecay=1e-4,Regularization=None,"
										  "DropConfig=0.0+0.5+0.5+0.5");

		// General Options.
		TString dnnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
						   "WeightInitialization=XAVIERUNIFORM");
		dnnOptions.Append(":");
		dnnOptions.Append(layoutString);
		dnnOptions.Append(":");
		dnnOptions.Append(trainingStrategyString);

		// Cuda implementation.
		if (Use["DNN_GPU"])
		{
			TString gpuOptions = dnnOptions + ":Architecture=GPU";
			factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_GPU", gpuOptions);
		}
		// Multi-core CPU implementation.
		if (Use["DNN_CPU"])
		{
			TString cpuOptions = dnnOptions + ":Architecture=CPU";
			factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);
		}
	}

	factory->TrainAllMethods();
	factory->TestAllMethods();
	factory->EvaluateAllMethods();
	outputFile->Close();

	std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
	std::cout << "==> TMVAClassification is done!" << std::endl;

	TFile *f = new TFile("TMVAfactory.root", "recreate");
	factory->Write();
	f->Close();

	delete factory;
	delete dataloader;

	// Launch the GUI for the root macros
	if (!gROOT->IsBatch())
		TMVA::TMVAGui(outfileName);
	return false;
}

void higgsPredictJet()
{
	TString methodList;
	returnState(methodList);
}