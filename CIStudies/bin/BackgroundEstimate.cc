#include <iostream>
#include <TH1.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TAxis.h>
#include <sstream>
#include <fstream>
#include <array>
#include "cstdio"
#include <iostream>
#include <string>
#include <cstdlib>
#include "../interface/TDisplayText.h"
//swap out_file to cout if you want to print to terminal. 
#define outout out_file

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


        Parameters(std::string line)
        {   
            std::string eventsString, massTargetString, crossSectionString, signalSwitchString, sumSwitchString; 
            std::istringstream str(line);
            std::getline(str, massTargetString, '\t');
            std::getline(str, fileName, '\t'); 
            std::getline(str, eventsString, '\t');
            std::getline(str, histograms, '\t');
            std::getline(str, crossSectionString, '\t');
            std::getline(str, signalSwitchString, '\t');
            std::getline(str, analysisName, '\t');
            std::getline(str, nickname, '\t');
            std::getline(str, sumSwitch, '\t');
            
            massTarget = std::stod(massTargetString);
            totalEvents = std::stod(eventsString);
            crossSection = std::stod(crossSectionString);
            signalSwitch = std::stod(signalSwitchString);
            sumSwitch = std::stod(sumSwitchString);
            //Change this from 4 to 3 depending on which lepton bin you want to analyze
            analysisName = "4" + analysisName;

        }
    };

void ComputationalFunction(Parameters param, std::ostream& out_file)
    {

        //Opens my file, assigns it f
        TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
        // outout << param.fileName;
        outout << param.nickname;
        outout << " ";
        TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
        

        //Takes the histogram wanted from the file, assigns it hist
        TH1 *hist = dynamic_cast<TH1*>(f->Get(param.histograms.c_str())); //Change this for different fit 
        if (!hist)
            throw std::runtime_error ("Hist not found");

        //Creates a power law fit function
        TF1 *fitfunc = new TF1("fitfunc", "[0]*pow(x, [1])", 200, 1300);

        //Fits the histogram with the fit function
        hist->Fit(fitfunc, "Q0", "", 200, 1300);

        //Finds the number of events in the analyzed hist
        TH1F *histanalysis = (TH1F*)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
        double eventsanalysishist = histanalysis->GetEntries();
        // outout << "\neventsanalysishist \n";
        // outout << eventsanalysishist;

        //Finds number of events ran total (from spreadsheet)
        double totaleventsran = param.totalEvents; //Change this for different event numbers
        // outout << "\ntotaleventsran \n";
        // outout << totaleventsran;

        //Finds lepton hist fraction
        double efficiency = eventsanalysishist/totaleventsran;
        // outout << "\nefficiency \n";
        // outout << efficiency;

        //Finds the integral of the fit from the mass target range +-5%
        double acceptedCenter = param.massTarget;
    
        
        double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
        double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
        double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);
        // outout << "\nacceptedfitintegral \n";
        // outout << acceptedfitintegral;
        
        //Finds the integral of the fit from 150-1300
        double rangefitintegral = fitfunc->Integral(150, 1300);
        // outout << "\nrangefitintegral \n";
        // outout << rangefitintegral;

        //Finds the integral of the analysis histogram from 0-1500
        double totalanalysishistintegral = histanalysis->Integral();
        // outout << "\ntotalanalysishistintegral \n";
        // outout << totalanalysishistintegral;

        //Finds the # of 4L events in the fit range
        double analysis150bin = histanalysis->FindBin(150);
        double analysis1300bin = histanalysis->FindBin(1300);
        double eventanalysisfitrange = (histanalysis->Integral(analysis150bin, analysis1300bin)/totalanalysishistintegral);
        // outout << "\neventanalysisfitrange \n";
        // outout << eventanalysisfitrange;

        //Calculates ratio between both fit integrals
        double fitratio = acceptedfitintegral/rangefitintegral;
        // outout << "\nfitratio \n";
        // outout << fitratio;

        //Finds the integral of the fit histogram from 0-1500
        double totalhistintegral = hist->Integral(0, 1500);
        // outout << "\ntotalhistintegral \n";
        // outout << totalhistintegral;

        // //Finds the integral of the fit histogram from 150-1300
        // double bin150 = hist->FindBin(150);
        // double bin1300 = hist->FindBin(1300);
        // double rangehistintegral = hist->Integral(bin150, bin1300);
        // outout << "\nrangehistintegral \n";
        // outout << rangehistintegral;

        //Finds histogram fraction
        double histfraction = fitratio*(eventanalysisfitrange);
        // outout << "\nhistfraction \n";
        // outout << histfraction;

        //Finds luminosity (from spreadsheet)
        double luminosity = 137;
        // outout << "\nluminosity \n";
        // outout << luminosity;

        //Finds crosssection (from spreadsheet)
        double crosssection = param.crossSection; //Change this for different cross section
        // outout << "\ncrosssection \n";
        // outout << crosssection;     


        //Finds backgroundest
        double backgroundest = efficiency*crosssection*1000*luminosity*histfraction;
        outout << backgroundest;
        outout << "\n";

        // if (param.sumSwitch == 1)
        //     oneSum = (backgroundest + oneSum);
        // if (param.sumSwitch == 2)
        //     twoSum = (backgroundest + twoSum);


        //f->Close();
        delete histanalysis;
        delete fitfunc;
        delete hist;
        delete f;

        
    }

void SignalFunction(Parameters param, std::ostream& out_file)
{
    //Opens my file, assigns it f
    TFile *f = new TFile(param.fileName.c_str()); //Change this for different files
    //outout << param.fileName;
    //outout << "\n";

    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1*>(f->Get(param.histograms.c_str())); //Change this for different fit 
    if (!hist)
        throw std::runtime_error ("Hist not found");

    //Finds the number of events in the analyzed hist
    TH1F *histanalysis = (TH1F*)f->Get(param.analysisName.c_str()); //Change this for different hist analysis
    double eventsanalysishist = histanalysis->GetEntries();
    // outout << "\neventsanalysishist \n";
    // outout << eventsanalysishist;

    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = param.totalEvents; //Change this for different event numbers
    // outout << "\ntotaleventsran \n";
    // outout << totaleventsran;

    //Finds lepton hist fraction
    double efficiency = eventsanalysishist/totaleventsran;
    // outout << "\nefficiency \n";
    // outout << efficiency;

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
    // outout << crosssection;

    //Finds the # of 4L events in the fit range
    double analysislowboundbin = histanalysis->FindBin(masslowaccepted);
    double analysisupboundbin = histanalysis->FindBin(masshighaccepted);
    double fraction4l = (histanalysis->Integral(analysislowboundbin, analysisupboundbin)/totalanalysishistintegral);
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

    outout << "\nMass "; 
        
    outout << acceptedCenter; 

    //Finds signalest
    double signalest = efficiency*crosssection*1000*luminosity*fraction4l;
    outout << "\nH++ Signal ";
    outout << signalest;
    outout << "\n";

    // if (param.sumSwitch == 1)
    //     oneSum = (signalest + oneSum);
    // if (param.sumSwitch == 2)
    //     twoSum = (signalest + twoSum);
        
}

int main()
{
    //Defines my text file with my events data as eventfilename
    const string eventfilename = "eventfiles.txt";
    //Defines my mass text file as massfilename
    const string massfilename = "masses.txt";
    //Reads from the file
    std::ifstream masssrc(massfilename);

    

    //Output file
    std::ofstream out_file;
    out_file.open("test.txt");

        //Runs the following loop while the file is open
        while (masssrc)
        {
            std::ifstream eventsrc(eventfilename);
            string massholder;
            string masslinenotarget;

            //Finds just the mass value and defines it massholder
            getline(masssrc, massholder, '\t');   
            //Finds the rest of line and calls it masslinenotarget         
            getline(masssrc, masslinenotarget);
            if (massholder.empty())
                break;

            
            std::vector<Parameters> eventArray;
            int eventnumRead = 0;

            eventArray.push_back(Parameters(massholder + '\t' + masslinenotarget));
            SignalFunction(eventArray.back(), out_file);

            //Runs the following loop while my file is open
            while (eventsrc) 
            {

                //Defines a string holder 
                string holder;
                //Gets a line from my events file and assigns it to holder
                getline(eventsrc, holder);
                //If the line is empty, end the loop
                if (holder.empty())
                    break;
                //Adds the line to my event array in the first 
                eventArray.push_back(Parameters(massholder + '\t' + holder));
                ComputationalFunction(eventArray.back(), out_file);
                
            }

            if (eventArray.empty())
                throw std::runtime_error("eventArray empty");
        

            //Reads my mass value, and then adds the rest of the line onto it, and feeds it into parameter to run it in the signal func


            


        }
    out_file.close();
    // outout << "\nSum of One Estimates\t";
    // outout << sumOne;
    // outout << "\nSum of Two Estimates\t";
    // outout << sumTwo;
}

