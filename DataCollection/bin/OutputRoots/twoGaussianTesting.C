#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"
#include <iostream>
#include <cmath>

void twoGaussianTesting() 
{

    // NOTE: AS OF 09/02, 300 has NO EVENTS AND WILL BE EXCLUDED
    char* arr[] = { "Run200.root", "Run500.root", "Run800.root", "Run1000.root", "Run1300.root" };
    int means[] = { 200, 500, 800, 1000, 1300 };

    // Values of quadratic term (second constant for wider value)
    double quad[] = { 17.894, 41.825, 51.554, 50.15, 36.209 };

    // Comment out loop at first
    // for(int k = 0; k < 5; k++) {
        int k = 2;
        auto f = arr[k];
        auto mean = means[k];

        double c0 = 130000.0 / means[k];
        double c1 = quad[k];

        double s0 = 0.00539 * means[k] + 1.54;
        double s1 = 0.0221 * means[k] + 5.75;

        std::cout << f << std::endl;
        // Root - error message is segfaulting
        TFile* file = TFile::Open(f);

        // histogram
        TH1* hist = file->Get<TH1>("Reco Same Sign Invariant Mass");

        if(hist)
        {

            // Predetermined function approximating the data
            std::string str = std::to_string(c0) + "*exp(-0.5*((x-"
                + std::to_string(means[k]) + ")/" + std::to_string(s0) + ")**2) + " 
                + std::to_string(c1) + "*exp(-0.5*((x-" + std::to_string(means[k]) 
                + ")/" + std::to_string(s1) + ")**2) + [0]";
            TF1 *fa = new TF1("fa", str.c_str(), mean*0.8, mean*1.2);

            // R - use range specified, S - save result to pointer
            // 0 - do not open graph, Q - do not print coefficients
            auto fit = hist->Fit(fa,"RS");
        }    
    // }
   
    // std::cout << hist->GetEntries() << std::endl;

    // std::cout << hist->GetBinContent(hist->FindFirstBinAbove(100)) << std::endl;
}