#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"
#include <iostream>
#include <cmath>

double crystalball_function(double x, double alpha, double n, double sigma, double mean) {
  // evaluate the crystal ball function
  if (sigma < 0.)     return 0.;
  double z = (x - mean)/sigma; 
  if (alpha < 0) z = -z; 
  double abs_alpha = std::abs(alpha);
  // double C = n/abs_alpha * 1./(n-1.) * std::exp(-alpha*alpha/2.);
  // double D = std::sqrt(M_PI/2.)*(1.+ROOT::Math::erf(abs_alpha/std::sqrt(2.)));
  // double N = 1./(sigma*(C+D));
  if (z  > - abs_alpha)
    return std::exp(- 0.5 * z * z);
  else {
    //double A = std::pow(n/abs_alpha,n) * std::exp(-0.5*abs_alpha*abs_alpha);
    double nDivAlpha = n/abs_alpha;
    double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
    double B = nDivAlpha -abs_alpha;
    double arg = nDivAlpha/(B-z);
    return AA * std::pow(arg,n);
  }
}

double crystalball_function(const double *x, const double *p) {
  // if ((!x) || (!p)) return 0.; // just a precaution
  // [Constant] * ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])
  return (p[0] * crystalball_function(x[0], p[3], p[4], p[2], p[1]));
}

void recoFitter() 
{

    // NOTE: AS OF 09/02, 300 has NO EVENTS AND WILL BE EXCLUDED
    char* arr[] = { "Run200.root", "Run500.root", "Run800.root", "Run1000.root", "Run1300.root" };
    int means[] = { 200, 500, 800, 1000, 1300 };

    // Comment out loop at first
    // for(int k = 0; k < 5; k++) {
        int k = 2;
        auto f = arr[k];
        auto mean = means[k];
        std::cout << f << std::endl;
        // Root - error message is segfaulting
        TFile* file = TFile::Open(f);

        // histogram
        TH1* hist = file->Get<TH1>("Reco Same Sign Invariant Mass");

        if(hist)
        {
            TF1 *fa = new TF1("fa", crystalball_function, 0.9*mean, 1.1*mean, 5);
            fa->SetParNames("Constant", "Mean", "Sigma", "Alpha", "N");

            fa->SetParameter(0,200.0);
            fa->SetParameter(1,mean);
            fa->SetParameter(2,15);
            fa->SetParameter(3,1);
            fa->SetParameter(4,1);

            // R - use range 100-800GeV, S - save result to pointer
            // 0 - do not open graph, Q - do not print coefficients
            auto fit = hist->Fit(fa,"RS");
        }    
    // }
   
    // std::cout << hist->GetEntries() << std::endl;

    // std::cout << hist->GetBinContent(hist->FindFirstBinAbove(100)) << std::endl;
}