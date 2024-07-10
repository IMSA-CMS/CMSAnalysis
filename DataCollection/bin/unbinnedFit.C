#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TSystem.h"

double pearson(double *x, double *par);
double func(double *x, double *par);

void unbinnedFit()
{
    std::string filename = "/eos/uscms/store/user/snowmass/Snowmass2021/DelphesNtuplizer/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_200PU/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_ntuple_9_1.root";
    TFile* file = TFile::Open(filename.c_str());
    TTree* tree = file->Get<TTree>("myana/mytree");

    TF1* fitfunc = new TF1("pearson", pearson, 0, 10, 1);
    fitfunc->SetParameter(0, 200);
    fitfunc->SetParLimits(0, 169.304, 2000);

    // TF1* function = new TF1("newFunction", func, 0, 10, 0);
    gSystem->CompileMacro("unbinnedFitTest.C");

    tree->Draw("unbinnedFitTest(muon_pt)");
    int result = tree->UnbinnedFit("pearson", "unbinnedFitTest(muon_pt)", "", "V");
    //std::cout << "result: " << result << '\n';
}

double pearson(double *x, double *par)
{
    double mass = par[0];
	double a = (1.011e-4)*std::pow(mass, 1.944) + 1.180;
	double m = (3.336e-6)*std::pow((mass - 1.537e2), 1.948) + 2.091;
	double mu = 1.011*mass - 1.698;
	double n = (-1.091e5)*std::pow((mass - 1.521e2), -.503) + 2.608e4;
	double result;

	double constant = n * std::tgamma(m) / (a * std::sqrt(M_PI) * std::tgamma(m - 0.5));
	result = constant * std::pow((1 + ((x[0]-mu)/a) * ((x[0]-mu)/a)), -m);
    // std::cout << "x[0]: " << x[0] << '\n';
    // std::cout << "a: " << a << '\n';
    // std::cout << "mass: " << mass << '\n';
    // std::cout << "function returns: " << result << '\n';
	return result;

}

double func(double *x, double* par)
{
    return x[0]*x[0];
}
