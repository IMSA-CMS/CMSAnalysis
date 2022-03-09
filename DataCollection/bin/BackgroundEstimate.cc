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
#include "../interface/TDisplayText.h"

//swap out_file to cout if you want to print to terminal.
#define outout std::cout //out_file 

//Running count of background for inclusive
double g_sumOne = 0;
double g_sumTwo = 0;
struct Parameters
{

    double massTarget;
    std::string fileName;
    double totalEvents;
    std::string histograms;
    double crossSection;
    double fitSwitch;
    std::string analysisName;
    std::string nickname;
    double sumSwitch;
    std::string analysis5LName;
    std::string d1fitName;
    double dimensionSwitch; 

    Parameters(std::string line)
    {
        //Parameter bank which is used to insert parameters from my file into the code where needed
        std::string eventsString, massTargetString, crossSectionString, fitSwitchString, sumSwitchString, dimensionSwitchString;
        std::istringstream str(line);
        std::getline(str, massTargetString, '\t');
        std::getline(str, fileName, '\t');
        std::getline(str, eventsString, '\t');
        std::getline(str, histograms, '\t');
        std::getline(str, crossSectionString, '\t');
        std::getline(str, fitSwitchString, '\t');
        std::getline(str, analysisName, '\t');
        std::getline(str, nickname, '\t');
        std::getline(str, sumSwitchString, '\t');
        std::getline(str, analysis5LName, '\t');
        std::getline(str, d1fitName, '\t');
        std::getline(str, dimensionSwitchString, '\t');

        massTarget = std::stod(massTargetString);
        totalEvents = std::stod(eventsString);
        crossSection = std::stod(crossSectionString);
        fitSwitch = std::stod(fitSwitchString);
        sumSwitch = std::stod(sumSwitchString);
        dimensionSwitch = std::stod(dimensionSwitchString);
        //Change this from 4 to 3 depending on which lepton bin you want to analyze
        //analysisName = "4" + analysisName;
        //d1analysisName = "4" + d1analysisName;
        //If you want to change bin now, just find and replace in eventfiles. Cut4 -> Cut5
    }
};

//Background estimate finding function
void ComputationalFunction(Parameters param, std::ostream &out_file)
{
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files

    //Reads out name if not an inclusive background
    if (param.sumSwitch == 0)
        {
        outout << param.nickname;
        outout << " ";
        }

    // outout << param.nickname;
    // outout << " ";

    //Find total events from file
    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);

    //Takes the fit histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(f->Get(param.histograms.c_str())); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Fit hist not found in file " + param.fileName);

    //Creates a power law fit function
    TF1 *fitfunc = new TF1("fitfunc", "[0]*pow((x + [2]), [1])", 150, 1300);

    //Root says to initialize these
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
    }

    //Pulls analysis hist from file
    TH1 *histanalysis = (TH1 *)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
    if (!histanalysis)
    {
        //outout << "Analysis hist not found in file " << param.fileName;
        //outout << "\n";
        return;
    }

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers

    if (param.fitSwitch == 0)
        {
        //Takes the integral of the analysis hist
        double eventsanalysishist = histanalysis->GetEntries();

        //Defines out mass range and takes the integral of the fit for that range
        double acceptedCenter = param.massTarget;
        double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
        double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
        double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);

        //Finds the integral of the fit from 150-1300
        double rangefitintegral = fitfunc->Integral(150, 1700);

        //Finds the integral of the analysis histogram from 0-1500
        double totalanalysishistintegral = histanalysis->Integral();

        //Finds the # of 4L events in the fit range divided by total events in the analysis hist
        double analysis150bin = histanalysis->FindBin(150);
        double analysis1300bin = histanalysis->FindBin(1700);
        double eventanalysisfitrange = exp(log(histanalysis->Integral(analysis150bin, analysis1300bin)) - log(totalanalysishistintegral));
    
        //Calculates ratio between both fit integrals
        double fitratio = exp(log(acceptedfitintegral) - log(rangefitintegral));

        //Finds histogram fraction
        double histfraction = exp(log(fitratio) + log(eventanalysisfitrange));

        //Finds luminosity (from spreadsheet)
        double luminosity = 3000;

        //Finds efficiency
        double efficiency = exp(log(eventsanalysishist) - log(totaleventsran));

        //Finds crosssection (from spreadsheet)
        double crosssection = param.crossSection; //Change this for different cross section

        //Finds background
        double backgroundest = exp(log(efficiency) + log(1000) + log(crosssection) + log(luminosity) + (2 * log(histfraction/2)));
        //For efficiency lose: log(1000) + log(crosssection) + log(luminosity)

        // double effnumb = exp(log(efficiency) + (2 * log(histfraction/2)));
        // outout << effnumb;
        // outout << "\n";

        //Prints non inclusive backgrounds
        if (param.sumSwitch == 0)
        {
            outout << backgroundest;
            outout << "\n";
        }

        //Adds my backgrounds if they need an inclusive at the end
        if (param.sumSwitch == 1)
        {
            g_sumOne = (g_sumOne + backgroundest);
        }
        if (param.sumSwitch == 2)
        {
            g_sumTwo = (g_sumTwo + backgroundest);
        }

        // outout << "\n";
        // outout << param.nickname;
        // outout << "\n";
        // outout << backgroundest;
        // outout << "\n";

        //Deletes leftovers
        delete histanalysis;
        delete fitfunc;
        delete hist;
        delete f;
        }

    if (param.fitSwitch == 1)
    {
        auto newAnalysis = dynamic_cast<TH2*>(histanalysis);
        //Takes the integral of the analysis hist
        double eventsinanalysishist = newAnalysis->GetEntries();

        //Finds the mass target range +-5%
        double acceptedCenter = param.massTarget;
        double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
        double masshighaccepted = acceptedCenter + (acceptedCenter * .05);

        //Finds the # of 4L events in the fit range divided by total # of events
        Int_t binlowx = newAnalysis->GetXaxis()->FindBin(masslowaccepted); 
        Int_t binlowy = newAnalysis->GetYaxis()->FindBin(masslowaccepted); 
        Int_t binhighx = newAnalysis->GetXaxis()->FindBin(masshighaccepted); 
        Int_t binhighy = newAnalysis->GetYaxis()->FindBin(masshighaccepted); 

        double acceptedanalysisintegral = ((newAnalysis->Integral(binlowx, binhighx, binlowy, binhighy)) / eventsinanalysishist);

        double histfraction = (acceptedanalysisintegral / eventsinanalysishist);
        //Finds luminosity (from spreadsheet)
        double luminosity = 3000;

        //Finds crosssection (from spreadsheet)
        double crosssection = param.crossSection; //Change this for different cross section

        //Finds efficiency
        double efficiency = exp(log(eventsinanalysishist) - log(totaleventsran));

        //Finds background
        double backgroundest = exp(log(efficiency) + log(1000) + log(crosssection) + log(luminosity) + log(histfraction));

        // double effnumb = efficiency * histfraction;
        // outout << effnumb;
        // outout << "\n";

        outout << backgroundest;
        outout << "\n";
    }
}

//Signal estimation function
void SignalFunction(Parameters param, std::ostream &out_file)
{
   
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files

    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(f->Get(param.histograms.c_str())); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Fit hist " +  param.histograms + " not found");

    //Finds the total number of events in the file
    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);
   
    //Changes the 4 to a 5 for my analysis hist so we have the 5 lepton hist as a variable
    std::string analysis4L = param.analysisName;
    std::string analysis5L = param.analysis5LName;
    // outout << analysis4L; 
    // outout << "\n";
    // outout << analysis5L; 
    // outout << "\n";
    
    //Defines the hists from the files
    TH2 *histanalysis = (TH2F *)f->Get(analysis4L.c_str()); //Change this for different hist analysis
    if (!histanalysis)
        throw std::runtime_error("4L Hist " +  param.analysisName + " not found");
    TH2 *histanalysis5L = (TH2F *)f->Get(analysis5L.c_str()); //Change this for different hist analysis
    if (!histanalysis5L)
        throw std::runtime_error("5L Hist " +  param.analysis5LName + " not found");
   
    //Adds the 4 lepton and 5 lepton bin hists together
    histanalysis->Add(histanalysis5L);

    //Finds the integral of the analysis histogram from 0-1500
    double eventsanalysishist = histanalysis->GetEntries();

    //Finds number of events ran total (from file)
    double totaleventsran = totalEventsInt; 
    
    //Finds efficiency
    double efficiency = eventsanalysishist / totaleventsran;

    //Finds luminosity (from spreadsheet)
    double luminosity = 3000;

    //Finds the mass target range +-5%
    double acceptedCenter = param.massTarget;
    double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
    double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
    // outout << masslowaccepted;
    // outout << "\n";
    // outout << masshighaccepted;
    // outout << "\n";
    //Finds crosssection (from spreadsheet)
    double crosssection = param.crossSection; //Change this for different cross section
  
    //Finds the # of 4L events in the fit range divided by total # of events
    Int_t binlowx = histanalysis->GetXaxis()->FindBin(masslowaccepted); 
    // outout << "\n";
    // outout << binlowx;
    // outout << "\n";
    Int_t binlowy = histanalysis->GetYaxis()->FindBin(masslowaccepted); 
    // outout << binlowy;
    // outout << "\n";

    Int_t binhighx = histanalysis->GetXaxis()->FindBin(masshighaccepted); 
    // outout << binhighx;
    // outout << "\n";
    Int_t binhighy = histanalysis->GetYaxis()->FindBin(masshighaccepted); 
    // outout << binhighy;
    // outout << "\n";

    // if(binlowx == binhighx)
    //     binhighx ++;

    // if(binlowy == binhighy)
    //     binhighy ++;

    double fraction4l = ((histanalysis->Integral(binlowx, binhighx, binlowy, binhighy)) / eventsanalysishist);

    //double fraction4l = ((histanalysis->Integral()) / eventsanalysishist);
    
    //Reads out mass
    outout << "\nMass ";
    outout << acceptedCenter;
   
    //Finds signalest
    double signalest = efficiency * fraction4l * crosssection * 1000 * luminosity;
    double signalest4 = signalest * 4;

    //Finds the other efficiency
    double effnumb = efficiency * fraction4l;
    //Reads out Efficiency Numbers
    // outout << "\nH++ Signal ";
    // outout << effnumb;
    // outout << "\n";

    //Reads out signal
    outout << "\nH++ Signal ";
    outout << signalest4;
    outout << "\n";
    
}

//Dark photon function
void LeptonJetComp(Parameters param, std::ostream &out_file)
{
    
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files

    //Reads out name if non-inclusive background
    if (param.sumSwitch == 0)
    {
        outout << param.nickname;
        outout << " ";
    }


    //Removes the 4 from my analysis hist
    param.analysisName.erase(remove(param.analysisName.begin(), param.analysisName.end(), '4'), param.analysisName.end());

    //Pulls analysis hist from file
    TH1F *histanalysis = (TH1F *)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
    if (!histanalysis)
        throw std::runtime_error("Analysis hist not found in file " + param.fileName);

    //Integral over entire analysis hist to find total events
    double totalhistintegral = histanalysis->Integral();
    
    //Finds the total # of events in the bin we want, 3, and then divides by total # of events. 
    double eventsin2bin = histanalysis->GetBinContent(3);
    double efficiency = exp(log(eventsin2bin) - log(totalhistintegral));

    
    //Finds luminosity (from spreadsheet)
    double luminosity = 3000;
   
    //Finds crosssection (from spreadsheet), 1 for dark photons
    double crosssection = 1; //Change this for different cross section

    //Finds backgroundest
    double backgroundest = exp(log(efficiency) + log(crosssection) + log(1000) + log(luminosity));

    //Reads out background if not inclusive 
    if (param.sumSwitch == 0)
    {
        outout << backgroundest;
        outout << "\n";
    }

    //Adds backgrounds to the count if inclusive
    if (param.sumSwitch == 1)
        g_sumOne = (g_sumOne + backgroundest);
    if (param.sumSwitch == 2)
        g_sumTwo = (g_sumTwo + backgroundest);

    //Deletes leftovers
    delete histanalysis;
    delete f;
}

//Main function which sends the data to the computational ones
int main()
{
    //Defines my text file with my events data
    const std::string eventfilename = "eventfiles.txt";
    //Defines my mass text file
    const std::string massfilename = "masses.txt";
    //Defines my lepton text file
    const std::string leptonfilename = "leptonjet.txt";
    //Defines my lepton background file
    const std::string leptonbgfilename = "leptonbackground.txt";
    //Reads from the file
    std::ifstream masssrc(massfilename);

    //Opems output file
    std::ofstream out_file;
    out_file.open("test.txt");

    //Runs the following loop while the file is open
    while (masssrc)
    {
        //Defines needed variables
        std::ifstream eventsrc(eventfilename);
        std::string massholder;
        std::string masslinenotarget;

        //Finds just the mass value and defines it massholder
        getline(masssrc, massholder, '\t');
        //Finds the rest of line and calls it masslinenotarget
        getline(masssrc, masslinenotarget);
        //Skips if empty or crossed out
        if (massholder.empty() || massholder[0] == '#')
            continue;

        //Defines array to hold the numbers which will be sent to parameters later
        std::vector<Parameters> eventArray;

        //Adds the variables processed from the file to the array
        eventArray.push_back(Parameters(massholder + '\t' + masslinenotarget));
        SignalFunction(eventArray.back(), out_file);

        //Runs the following loop while my file is open, this is for background instead of signal
        while (eventsrc)
        {

            //Defines a string holder
            std::string holder;
            //Gets a line from my events file and assigns it to holder
            getline(eventsrc, holder);
            //If the line is empty, skip
            if (holder.empty() || holder[0] == '#')
                continue;
            
            //Adds the line to my event array in the first spot
            eventArray.push_back(Parameters(massholder + '\t' + holder));
            ComputationalFunction(eventArray.back(), out_file);
        }

        //Prints out my inclusive backgrounds from the previous mass, then sets them to 0 for the next run
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
    //Closes the mass file
    masssrc.close();

    //Defines a new array for leptons
    std::vector<Parameters> leptonArray;

    //Reads in lepton file
    std::ifstream leptonsrc(leptonfilename);
    while (leptonsrc)
    {
        //Defines variables for later
        outout << "\n";
        std::string leptonmassholder;
        std::string leptonmasslinenotarget;

        //Finds just the mass value and defines it leptonmassholder
        getline(leptonsrc, leptonmassholder, '\t');
        //Finds the rest of line and calls it leptonmasslinenotarget
        getline(leptonsrc, leptonmasslinenotarget);
        if (leptonmassholder.empty() || leptonmassholder[0] == '#')
            continue;

        //Adds those two holders to the array, sends it to be defined as parameters, then sends it into my dark photon function
        leptonArray.push_back(Parameters(leptonmassholder + '\t' + leptonmasslinenotarget));
        LeptonJetComp(leptonArray.back(), out_file);

        //Reads in the lepton background file
        std::ifstream leptonbgsrc(leptonbgfilename);
        if (!leptonbgsrc)
            throw std::runtime_error("No lepton background file");

        //Runs the following loop while my file is open
        while (leptonbgsrc)
        {
        
            //Defines a string leptonholder
            std::string leptonholder;

            //Gets a line from my background file
            getline(leptonbgsrc, leptonholder);

            //If the line is empty, continue
            if (leptonholder.empty() || leptonholder[0] == '#')
                continue;

            //Adds the line to my event array in the first
            leptonArray.push_back(Parameters(leptonmassholder + '\t' + leptonholder));
            LeptonJetComp(leptonArray.back(), out_file);
        }

        //Reads out the inclusive backgrounds and then sets them to 0
        outout << "ZtoLL Inclusive ";
        outout << g_sumOne;
        outout << "\nQCD Inclusive ";
        outout << g_sumTwo;
        outout << "\n";
        g_sumOne = 0;
        g_sumTwo = 0;
    }

    out_file.close();
}
