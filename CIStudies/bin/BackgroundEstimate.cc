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
#include "TH2.h"
#include "TH2F.h"
// #include <cmath>

// #include "Minuit2/Minuit2Minimizer.h"

// #include "Math/IFunction.h"
// #include "Math/IOptions.h"

// #include "Fit/ParameterSettings.h"

// #include "Minuit2/FCNAdapter.h"
// #include "Minuit2/FumiliFCNAdapter.h"
// #include "Minuit2/FCNGradAdapter.h"
// #include "Minuit2/FunctionMinimum.h"
// #include "Minuit2/MnMigrad.h"
// #include "Minuit2/MnMinos.h"
// #include "Minuit2/MinosError.h"
// #include "Minuit2/MnHesse.h"
// #include "Minuit2/MinuitParameter.h"
// #include "Minuit2/MnUserFcn.h"
// #include "Minuit2/MnPrint.h"
// #include "Minuit2/VariableMetricMinimizer.h"
// #include "Minuit2/SimplexMinimizer.h"
// #include "Minuit2/CombinedMinimizer.h"
// #include "Minuit2/ScanMinimizer.h"
// #include "Minuit2/FumiliMinimizer.h"
// #include "Minuit2/MnParameterScan.h"
// #include "Minuit2/MnContours.h"
// #include "Minuit2/MnTraceObject.h"
// #include "Minuit2/MinimumBuilder.h"

// #include <cassert>
// #include <iostream>
// #include <algorithm>
// #include <functional>
// #include "TROOT.h"
// #include "TMinuit2TraceObject.h"
//#include "MinimizerOptions.h"
#include "../interface/TDisplayText.h"

//swap out_file to cout if you want to print to terminal.
#define outout std::cout //out_file 

double g_sumOne = 0;
double g_sumTwo = 0;
int histcounter = 0;
struct Parameters
{

    double massTarget;
    std::string fileName;
    double totalEvents;
    std::string histograms;
    double crossSection;
    double signalSwitch;
    std::string analysisName;
    std::string nickname;
    double sumSwitch;
    std::string d1analysisName;
    std::string d1fitName;
    double dimensionSwitch; 

    Parameters(std::string line)
    {
        std::string eventsString, massTargetString, crossSectionString, signalSwitchString, sumSwitchString, dimensionSwitchString;
        std::istringstream str(line);
        std::getline(str, massTargetString, '\t');
        std::getline(str, fileName, '\t');
        std::getline(str, eventsString, '\t');
        std::getline(str, histograms, '\t');
        std::getline(str, crossSectionString, '\t');
        std::getline(str, signalSwitchString, '\t');
        std::getline(str, analysisName, '\t');
        std::getline(str, nickname, '\t');
        std::getline(str, sumSwitchString, '\t');
        std::getline(str, d1analysisName, '\t');
        std::getline(str, d1fitName, '\t');
        std::getline(str, dimensionSwitchString, '\t');

        massTarget = std::stod(massTargetString);
        totalEvents = std::stod(eventsString);
        crossSection = std::stod(crossSectionString);
        signalSwitch = std::stod(signalSwitchString);
        sumSwitch = std::stod(sumSwitchString);
        dimensionSwitch = std::stod(dimensionSwitchString);
        //Change this from 4 to 3 depending on which lepton bin you want to analyze
        analysisName = "4" + analysisName;
        d1analysisName = "4" + d1analysisName;
    }
};

void ComputationalFunction(Parameters param, std::ostream &out_file)
{
    //auto min_opts = ROOT::Math::MinimizerOptions();

    //min_opts.SetMinimizerAlgorithm("Fumili");

    // ROOT::Math::MinimizerOptions::SetMinimizerAlgorithm("Fumili");

    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
    // outout << param.fileName;
    if (param.sumSwitch == 0)
        {
        outout << param.nickname;
        outout << " ";
        }


    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);

    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(f->Get(param.histograms.c_str())); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Fit hist not found in file " + param.fileName);

    //Creates a power law fit function
    TF1 *fitfunc = new TF1("fitfunc", "[0]*pow((x + [2]), [1])", 150, 1300);

    // Root says to initialize these
    fitfunc->SetParameters(1e9, -3, 100);

    //Fits the histogram with the fit function
    // Root's fitting is lousy, so we have to do it again and again
    // One would think a minimization algorithm could do this itself?
    double func = std::numeric_limits<double>::max();
    const double tolerance = .1;
    while (true)
    {
        auto result = hist->Fit(fitfunc, "RBSQ0", "", 150, 1300);
        double newVal = result->MinFcnValue();
        if (func - newVal > tolerance)
        {
            func = newVal;
        }
        else
        {
            break;
        }
        // hist->Fit(fitfunc, "RB", "", 150, 1300);
        // hist->Fit(fitfunc, "RB", "", 150, 1300);
    }
    //TCanvas *canvas = new TCanvas((histcounter + "Canvas"), "dCanvas", 1000,1000);
    // histcounter = histcounter + 1;

    //Finds the number of events in the analyzed hist
    TH1F *histanalysis = (TH1F *)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
    if (!histanalysis)
        throw std::runtime_error("Analysis hist not found in file " + param.fileName);
    double eventsanalysishist = histanalysis->GetEntries();
    //outout << "\neventsanalysishist \n";
    //outout << eventsanalysishist;

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers
    //outout << "\ntotaleventsran \n";
    //outout << totaleventsran;

    //Finds lepton hist fraction
    double efficiency = exp(log(eventsanalysishist) - log(totaleventsran));
    //outout << "\nefficiency \n";
    //outout << efficiency;

    //Finds the integral of the fit from the mass target range +-5%
    double acceptedCenter = param.massTarget;

    double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
    double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
    double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);
    // outout << "\nacceptedfitintegral \n";
    //BAD BAD BAD!
    //outout << "\nAcceptedFitIntegral\n";
    //outout << acceptedfitintegral;

    //Finds the integral of the fit from 150-1300
    double rangefitintegral = fitfunc->Integral(150, 1300);
    // outout << "\nrangefitintegral \n";
    //outout << "\nRangeFitInt\n";
    //outout << rangefitintegral;

    //Finds the integral of the analysis histogram from 0-1500
    double totalanalysishistintegral = histanalysis->Integral();
    //outout << "\nTotalAnalysisHistInt \n";
    //outout << totalanalysishistintegral;

    //Finds the # of 4L events in the fit range
    double analysis150bin = histanalysis->FindBin(150);
    double analysis1300bin = histanalysis->FindBin(1300);
    double eventanalysisfitrange = exp(log(histanalysis->Integral(analysis150bin, analysis1300bin)) - log(totalanalysishistintegral));
    //outout << "\neventanalysisfitrange \n";

    //outout << eventanalysisfitrange;

    //Calculates ratio between both fit integrals
    double fitratio = exp(log(acceptedfitintegral) - log(rangefitintegral));
    
    // outout << "\nfitratio \n";
    // outout << fitratio;

    //Finds the integral of the fit histogram from 0-1500
    // double totalhistintegral = hist->Integral(0, 1500);
    //outout << "\ntotalhistintegral \n";
    //outout << totalhistintegral;

    // //Finds the integral of the fit histogram from 150-1300
    // double bin150 = hist->FindBin(150);
    // double bin1300 = hist->FindBin(1300);
    // double rangehistintegral = hist->Integral(bin150, bin1300);
    // outout << "\nrangehistintegral \n";
    // outout << rangehistintegral;

    //Finds histogram fraction
    double histfraction = exp(log(fitratio) + log(eventanalysisfitrange));
    // outout << "\nhistfraction\n";
    // outout << histfraction;
    // outout << "\n";

    //Finds luminosity (from spreadsheet)
    double luminosity = 3000;
    //outout << "\nluminosity \n";
    //outout << luminosity;

    //Finds crosssection (from spreadsheet)
    double crosssection = param.crossSection; //Change this for different cross section
    //outout << "\ncrosssection \n";
    //outout << crosssection;
    //outout << "\n";

    //hist->Draw("Hist");
    //canvas->Draw("Canvas");

    //Finds backgroundest
    double backgroundest = exp(log(efficiency) + log(crosssection) + log(1000) + log(luminosity) + (2 * log(histfraction/2)));
    if (param.sumSwitch == 0)
    {
        outout << backgroundest;
        outout << "\n";
    }
    // outout << param.nickname;
    // outout << "\n";
    // outout << backgroundest;
    // outout << "\n";

    if (param.sumSwitch == 1)
    {
        // outout << g_sumOne;
        // outout << "\n";
        g_sumOne = (g_sumOne + backgroundest);
    }
    if (param.sumSwitch == 2)
    {
        g_sumTwo = (g_sumTwo + backgroundest);
    }

    //f->Close();
    delete histanalysis;
    delete fitfunc;
    delete hist;
    delete f;
}

void SignalFunction(Parameters param, std::ostream &out_file)
{
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
    //outout << param.fileName;
    //outout << "\n";
    //4l->Add(5L)
    //for the hist. 
    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(f->Get(param.histograms.c_str())); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Hist not found");

    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);
    std::string analysis4L = param.analysisName;
    param.analysisName.erase(remove(param.analysisName.begin(), param.analysisName.end(), '4'), param.analysisName.end());
    std::string analysis5L = ("5" + param.analysisName);
    // outout << "\n";
    // outout << analysis4L;
    // outout << "\n";
    // outout << analysis5L;
    // outout << "\n";


    //Finds the number of events in the analyzed hist
    TH2 *histanalysis = (TH2F *)f->Get(analysis4L.c_str()); //Change this for different hist analysis
    TH2 *histanalysis5L = (TH2F *)f->Get(analysis5L.c_str()); //Change this for different hist analysis

    histanalysis->Add(histanalysis5L);

    double eventsanalysishist = histanalysis->GetEntries();
    // outout << "\neventsanalysishist \n";
    //outout << eventsanalysishist;

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers
    // outout << "\ntotaleventsran \n";
    //outout << totaleventsran;

    //Finds lepton hist fraction
    double efficiency = eventsanalysishist / totaleventsran;
    // outout << "\nefficiency \n";
    //outout << efficiency;

    //Finds the integral of the analysis histogram from 0-1500
    double totalanalysishistintegral = histanalysis->Integral();
    //Finds luminosity (from spreadsheet)
    double luminosity = 3000;
    // outout << "\nluminosity \n";
    // outout << luminosity;

    //Finds the integral of the fit from the mass target range +-5%
    double acceptedCenter = param.massTarget;
    double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
    double masshighaccepted = acceptedCenter + (acceptedCenter * .05);

    //Finds crosssection (from spreadsheet)
    double crosssection = param.crossSection; //Change this for different cross section
    // outout << "\ncrosssection \n";
    //outout << crosssection;

    // Int_t binlowlow = histanalysis->FindBin(masslowaccepted, masslowaccepted); 
    // outout << "\n";
    // outout << binlowlow;

    // Int_t binhighlow = histanalysis->FindBin(masshighaccepted, masslowaccepted); 

    // Int_t binlowhigh = histanalysis->FindBin(masslowaccepted, masshighaccepted); 
    
    // Int_t binhighhigh = histanalysis->FindBin(masshighaccepted, masshighaccepted); 
    
    // outout << "binhighigh ";
    // outout << binhighhigh;
    


    //Finds the # of 4L events in the fit range

     Int_t binlowx = histanalysis->GetXaxis()->FindBin(masslowaccepted); 
     Int_t binlowy = histanalysis->GetYaxis()->FindBin(masslowaccepted); 
    // Int_t binlowlow = histanalysis->GetBin(binlowlowx,binlowlowy,0);
    //  outout << "binlowx ";
    //  outout << binlowx;
    //  outout << "\n";
    //  outout << "binlowy ";
    //  outout << binlowy;
    //  outout << "\n";
     Int_t binhighx = histanalysis->GetXaxis()->FindBin(masshighaccepted); 
     Int_t binhighy = histanalysis->GetYaxis()->FindBin(masshighaccepted); 
    //  outout << "binhighx ";
    //  outout << binhighx;
    //  outout << "\n";
    //  outout << "binhighy ";
    //  outout << binhighy;
    //  outout << "\n";
    // Int_t binhighlow = histanalysis->GetBin(binhighlowx,binhighlowy,0);

    // Int_t binlowhighx = histanalysis->GetXaxis(masslowaccepted)->FindBin(masslowaccepted); 
    // Int_t binlowhighy = histanalysis->GetYaxis()->FindBin(masshighaccepted); 
    // Int_t binlowhigh = histanalysis->GetBin(binlowhighx,binlowhighy,0);

    // Int_t binhighhighx = histanalysis->GetXaxis()->FindBin(masshighaccepted); 
    // Int_t binhighhighy = histanalysis->GetYaxis()->FindBin(masshighaccepted); 
    // Int_t binhighhigh = histanalysis->GetBin(binhighhighx, binhighhighy, 0);
    double fraction4l = ((histanalysis->Integral(binlowx, binhighx, binlowy, binhighy)) / totalanalysishistintegral);
    // outout << binhighhigh;
    // double fraction4l = ((histanalysis->Integral(binlowlow, binhighlow, binlowhigh, binhighhigh)) / totalanalysishistintegral);
    // int analysislowlowboundbin = histanalysis->FindBin(masslowaccepted, masslowaccepted);
    // outout << analysislowlowboundbin;
    // outout << "\n";
    // int analysishighlowboundbin = histanalysis->FindBin(masshighaccepted, masslowaccepted);
    // outout << analysishighlowboundbin;
    // outout << "\n";
    // int analysislowhighboundbin = histanalysis->FindBin(masslowaccepted, masshighaccepted);
    // outout << analysislowhighboundbin;
    // outout << "\n";
    // int analysishighhighboundbin = histanalysis->FindBin(masshighaccepted, masshighaccepted);
    // outout << analysishighhighboundbin;
    // outout << "\n";
    // outout << (histanalysis->Integral(analysislowlowboundbin, analysishighlowboundbin, analysislowhighboundbin, analysishighhighboundbin) / totalanalysishistintegral);
    // outout << "\n";
    
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
    // outout << "\n";
    // outout << "fraction 4l ";
    // outout << fraction4l;
    // outout << "\n";

    outout << "\nMass ";

    outout << acceptedCenter;

    //Finds signalest
    double signalest = efficiency * crosssection * 1000 * luminosity * fraction4l;
    outout << "\nH++ Signal ";
    outout << signalest;
    outout << "\n";

 
}

void LeptonJetComp(Parameters param, std::ostream &out_file)
{
    
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
    // outout << param.fileName;
    outout << param.nickname;
    outout << " ";

    param.analysisName.erase(remove(param.analysisName.begin(), param.analysisName.end(), '4'), param.analysisName.end());
    


    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);


    //Finds the number of events in the analyzed hist
    TH1F *histanalysis = (TH1F *)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
    if (!histanalysis)
        throw std::runtime_error("Analysis hist not found in file " + param.fileName);
    
    //outout << "\neventsanalysishist \n";
    //outout << eventsanalysishist;

    //Finds the # of 4L events in the fit range
    double eventsin2bin = histanalysis->GetBinContent(3);
    double efficiency = exp(log(eventsin2bin) - log(totalEventsInt));
    //outout << "\neventanalysisfitrange \n";

    //Finds number of events ran total (from spreadsheet)
    // double totaleventsran = totalEventsInt; //Change this for different event numbers
    //outout << "\ntotaleventsran \n";
    //outout << totaleventsran;


    //Finds the integral of the analysis histogram from 0-1500
    // double totalanalysishistintegral = histanalysis->Integral();
    //outout << "\nTotalAnalysisHistInt \n";
    //outout << totalanalysishistintegral;

    

    //Finds luminosity (from spreadsheet)
    double luminosity = 3000;
    //outout << "\nluminosity \n";
    //outout << luminosity;

    //Finds crosssection (from spreadsheet)
    double crosssection = 1; //Change this for different cross section
    //outout << "\ncrosssection \n";
    //outout << crosssection;
    //outout << "\n";

    //hist->Draw("Hist");
    //canvas->Draw("Canvas");

    //Finds backgroundest
    double backgroundest = (log(efficiency) + log(crosssection) + log(1000) + log(luminosity));
    outout << backgroundest;
    outout << "\n";

    //f->Close();
    delete histanalysis;
    delete f;
}

int main()
{
    //Defines my text file with my events data as eventfilename
    const std::string eventfilename = "eventfiles.txt";
    //Defines my mass text file as massfilename
    const std::string massfilename = "masses.txt";
    //Defines my lepton text file as leptonjet.txt
    const std::string leptonfilename = "leptonjet.txt";
    const std::string leptonbgfilename = "leptonbg.txt";
    //Reads from the file
    std::ifstream masssrc(massfilename);

    //Output file
    std::ofstream out_file;
    out_file.open("test.txt");

    //Runs the following loop while the file is open
    while (masssrc)
    {
        std::ifstream eventsrc(eventfilename);
        std::string massholder;
        std::string masslinenotarget;

        //Finds just the mass value and defines it massholder
        getline(masssrc, massholder, '\t');
        //Finds the rest of line and calls it masslinenotarget
        getline(masssrc, masslinenotarget);
        if (massholder.empty() || massholder[0] == '#')
            continue;

        std::vector<Parameters> eventArray;

        eventArray.push_back(Parameters(massholder + '\t' + masslinenotarget));
        SignalFunction(eventArray.back(), out_file);

        //Runs the following loop while my file is open
        while (eventsrc)
        {

            //Defines a string holder
            std::string holder;
            //Gets a line from my events file and assigns it to holder
            getline(eventsrc, holder);
            //If the line is empty, end the loop
            if (holder.empty() || holder[0] == '#')
                continue;
            //Adds the line to my event array in the first
            eventArray.push_back(Parameters(massholder + '\t' + holder));
            ComputationalFunction(eventArray.back(), out_file);
        }

        outout << "ZtoLL Inclusive ";
        outout << g_sumOne;
        outout << "\nQCD Inclusive ";
        outout << g_sumTwo;
        outout << "\n";
        g_sumOne = 0;
        g_sumTwo = 0;

        if (eventArray.empty())
            throw std::runtime_error("eventArray empty");
    }
    std::vector<Parameters> eventArray;
    std::ifstream leptonsrc(leptonfilename);
    while (leptonsrc)
    {
        std::ifstream leptonbgsrc(leptonbgfilename);
        std::string massholder;
        std::string masslinenotarget;

        //Finds just the mass value and defines it massholder
        getline(leptonbgsrc, massholder, '\t');
        //Finds the rest of line and calls it masslinenotarget
        getline(leptonbgsrc, masslinenotarget);
        if (massholder.empty() || massholder[0] == '#')
            continue;

        std::vector<Parameters> eventArray;

        eventArray.push_back(Parameters(massholder + '\t' + masslinenotarget));
        LeptonJetComp(eventArray.back(), out_file);

        //Runs the following loop while my file is open
        while (leptonbgsrc)
        {
            outout << "lbginstance";
            outout << "\n";
            //Defines a string holder
            std::string holder;
            //Gets a line from my events file and assigns it to holder
            getline(leptonbgsrc, holder);
            //If the line is empty, end the loop
            if (holder.empty() || holder[0] == '#')
                continue;
            //Adds the line to my event array in the first
            eventArray.push_back(Parameters(massholder + '\t' + holder));
            LeptonJetComp(eventArray.back(), out_file);
        }
    }




    out_file.close();
}
