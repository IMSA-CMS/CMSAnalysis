#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"
#include <iostream>
#include <cmath>

void sumOfTwoGaussians() 
{

    // NOTE: AS OF 09/02, 300 has NO EVENTS AND WILL BE EXCLUDED
    char* arr[] = { "Run200.root", "Run500.root", "Run800.root", "Run1000.root", "Run1300.root" };
    int means[] = { 200, 500, 800, 1000, 1300 };

    // NOTE: AS OF 09/03, WE ARE FITTING VERY NARROW RANGE TO DETERMINE THE USEFULNESS OF MODELING

    // Comment out loop at first
    for(int k = 0; k < 5; k++) {
        // int k = 0;
        auto f = arr[k];
        auto mean = means[k];
        std::cout << f << std::endl;
        // Root - error message is segfaulting
        TFile* file = TFile::Open(f);

        // histogram
        TH1* hist = file->Get<TH1>("Reco Same Sign Invariant Mass");

        if(hist)
        {
            // Narrow range for fit
            // THESE PARAMETERS ARE GOLDEN DON'T TOUCH - ANDY, 09/03
            TF1 *fa = new TF1("fa", "gaus(0) + gaus(3)", mean*0.8, mean*1.2);

            /*
            fa->SetParameter(0,300.0);
            fa->SetParameter(1,800.0);
            fa->SetParameter(2,2.0);
            */

            // Set parameters that can be easily guessed
            // Means
            fa->SetParameter(1, mean);
            fa->SetParameter(4, mean);

            // Standard deviations (narrow and wide)
            fa->SetParameter(2, mean*0.01);
            fa->SetParameter(5, mean*0.05);

            // R - use range 100-800GeV, S - save result to pointer
            // 0 - do not open graph, Q - do not print coefficients
            auto fit = hist->Fit(fa,"0RS");
        }    
    }
   
    // std::cout << hist->GetEntries() << std::endl;

    // std::cout << hist->GetBinContent(hist->FindFirstBinAbove(100)) << std::endl;
}