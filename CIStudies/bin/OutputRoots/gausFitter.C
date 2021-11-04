#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"
#include <iostream>
#include <cmath>

void gausFitter() 
{

    // NOTE: AS OF 09/02, 300 has NO EVENTS AND WILL BE EXCLUDED
    char* arr[] = { "Run200.root", "Run500.root", "Run800.root", "Run1000.root", "Run1300.root" };
    int means[] = { 200, 500, 800, 1000, 1300 };

    // NOTE: AS OF 09/03, WE ARE FITTING VERY NARROW RANGE TO DETERMINE THE USEFULNESS OF MODELING

    // Comment out loop at first
    for(int k = 0; k < 5; k++) {
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
            TF1 *fa = new TF1("fa", "gaus(0)", mean*0.98, mean*1.02);

            /*
            fa->SetParameter(0,300.0);
            fa->SetParameter(1,800.0);
            fa->SetParameter(2,2.0);
            */

            // R - use range 100-800GeV, S - save result to pointer
            // 0 - do not open graph, Q - do not print coefficients
            auto fit = hist->Fit(fa,"RS");
        }    
    }
   
    // std::cout << hist->GetEntries() << std::endl;

    // std::cout << hist->GetBinContent(hist->FindFirstBinAbove(100)) << std::endl;
}