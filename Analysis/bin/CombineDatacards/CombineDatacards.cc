#include <iostream>
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFitResult.h"
#include <sstream>
#include <fstream>
#include <array>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"

int CombineDatacards(){
        HiggsPlusPlusAnalysis analysisObj;
        auto channels = analysisObj.getChannels();

     for(auto channel : channels)
        {
            channel->CombineDatacard(channel);
        }        
        return 0;
}
