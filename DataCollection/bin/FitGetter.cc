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
        auto result = hist->Fit(fitfunc, "RBS0", "", 150, 1300);
        //Removed Q from RBS0 to print out params. 
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
}

//Main function which sends the data to the computational ones
int FitGetter()
{
    //Defines my text file with my events data
    const std::string eventfilename = "eventfiles.txt";
    //Defines my mass text file
    const std::string massfilename = "masses.txt";
    //Reads from the file
    std::ifstream masssrc(massfilename);

    //Opens output file
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

        if (eventArray.empty())
            throw std::runtime_error("eventArray empty");

    }
    //Closes the mass file
    masssrc.close();

    out_file.close();
}
