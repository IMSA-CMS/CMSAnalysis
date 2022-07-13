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
#include <iostream>
#include <string>
#include <cstdlib>
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"


double WindowEstimator::getExpectedYield(const SingleProcess* process, double luminosity) const
{
    //Opens my file, assigns it f
    //TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
    //outout << param.fileName;
    //outout << "\n";

    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(process->getHist()); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Hist not found");

    // TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    // std::string totalEventsStr = totalevents->GetString().Data();
    // int totalEventsInt = std::stoi(totalEventsStr);

    int totalEventsInt = process->getTotalEvents();

    //Finds the number of events in the analyzed hist
    TH1F *histanalysis = (TH1F *)process->getHist(); //Change this for different hist analysis
    double eventsanalysishist = histanalysis->GetEntries();
    // outout << "\neventsanalysishist \n";
    //outout << eventsanalysishist;
    //std::cout << "GetEntries is " << std::to_string(eventsanalysishist) << std::endl;
    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers
    // outout << "\ntotaleventsran \n";
    //outout << totaleventsran;
    //std::cout << std::to_string(totaleventsran) << "      " << std::to_string(totalEventsInt) << std::endl; //correct
    //Finds lepton hist fraction
    double efficiency = eventsanalysishist / totaleventsran;
    //std::cout << std::to_string(efficiency) << " efficiency" << std::endl; 
    // outout << "\nefficiency \n";
    //outout << efficiency;

    //Finds the integral of the analysis histogram from 0-1500
    double totalanalysishistintegral = histanalysis->Integral();
    //std::cout << "integral " << std::to_string(totalanalysishistintegral) << std::endl;

    //Finds luminosity (from spreadsheet)
   
    // outout << "\nluminosity \n";
    // outout << luminosity;

    //Finds the integral of the fit from the mass target range +-5%
    //double acceptedCenter = process.massTarget;
    double masslowaccepted = lowBound;
    double masshighaccepted = highBound;

    //Finds crosssection (from spreadsheet)
    double crosssection = process->getCrossSection(); //Change this for different cross section
    // outout << "\ncrosssection \n";
    //outout << crosssection;
    //std::cout << "crosssection" << crosssection << std::endl;
    //Finds the # of 4L events in the fit range
    double analysislowboundbin = histanalysis->FindBin(masslowaccepted);
    //std::cout << "low bin " << analysislowboundbin << std::endl;
    double analysisupboundbin = histanalysis->FindBin(masshighaccepted);
    //std::cout << "high bin " << analysisupboundbin << std::endl;
    double fraction4l = (histanalysis->Integral(analysislowboundbin, analysisupboundbin) / totalanalysishistintegral);
    //std::cout << "within window" << std::to_string(histanalysis->Integral(analysislowboundbin, analysisupboundbin)) << std::endl;
    //std::cout << "fraction within window" << std::to_string(fraction4l) << std::endl;
    // outout << "hist integral thing";
    // outout << "\n";
    // outout << histanalysis->Integral(analysislowboundbin, analysisupboundbin);
    // outout << "\n";
    // outout << "Analysis low bound bin";
    // outout << "\n";
    // outout << analysislowboundbin;
    // outout << "\n";
    // outout << "Fraction4L";
    // outout << "\n";
    // outout << fraction4l;
    // outout << "\n";

    //outout << "\nMass ";

    //outout << acceptedCenter;

    //Finds signalest
    double signalest = efficiency * crosssection * 1000 * luminosity * fraction4l;
    //outout << "\nH++ Signal ";
    return signalest;
    //outout << "\n";

    // if (param.sumSwitch == 1)
    //     oneSum = (signalest + oneSum);
    // if (param.sumSwitch == 2)
    //     twoSum = (signalest + twoSum);
}