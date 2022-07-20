#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include <string>
#include <vector>

class Estimator
{
public: 

    std::string getFileName() const {return fileName;}
    double getMassTarget() const {return massTarget;}
    double getTotalEvents() const {return totalEvents;}
    std::string getHistogramName() const {return histogramName;}
    double getCrossSection() const {return histogramName;}
    bool getSignalSwitch() const {return signalSwitch;}
    std::string getAnalysisName() const {return analysisName;}
    std::string getNickname() const {return nickname;}
    double getSumSwitch() const {return sumSwitch;}

private:
    const std::string fileName;
    const double massTarget;
    const double totalEvents;
    const std::string histogramName;
    const double crossSection;
    const bool signalSwitch;
    const std::string analysisName; 
    const std::string nickname;
    const double sumSwitch;
};
class ProcessInput;

//SingleProcess Class
class SingleProcess
{
public:
    std::string getName() const {return name;} //Inlining
    double crossSection() const {return crossSection;}
    

private:
    const std::string name;
    const double crossSection;
    const ProcessInput* input;
    Estimator* estimator; 

};

    //Opens my file, assigns it f
    TFile *f = new TFile(getFileName().c_str()); //Change this for different files

    //Reads out name if not an inclusive background
    if (getSumSwitch() == 0)
        {
        outout << getNickname();
        outout << " ";
        }

    //Find total events from file
    TDisplayText *totalevents = f->Get<TDisplayText>("NEvents");
    std::string totalEventsStr = totalevents->GetString().Data();
    int totalEventsInt = std::stoi(totalEventsStr);

    //Takes the fit histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(f->Get(getHistogramName().c_str())); //Change this for different fit
    if (!hist)
        throw std::runtime_error("Fit hist not found in file " + getFileName());

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

    TH1 *histanalysis = (TH1 *)f->Get(getAnalysisName().c_str()); //Change this for different hist analysis
    if (!histanalysis)
    {
        //outout << "Analysis hist not found in file " << param.fileName;
        //outout << "\n";
        return;
    }
    
    //Finds number of events ran total (from spreadsheet)
    double totaleventsran = totalEventsInt; //Change this for different event numbers

    if (getFitSwitch() == 0)
        {
        //Takes the integral of the analysis hist
        double eventsanalysishist = histanalysis->GetEntries();

        //Defines out mass range and takes the integral of the fit for that range
        double acceptedCenter = getMassTarget();
        double masslowaccepted = acceptedCenter - (acceptedCenter * .05);
        double masshighaccepted = acceptedCenter + (acceptedCenter * .05);
        double acceptedfitintegral = fitfunc->Integral(masslowaccepted, masshighaccepted);

        //Finds the integral of the fit from 150-1300
        double rangefitintegral = fitfunc->Integral(150, 1700);
        
        //Finds the integral of the analysis histogram from 0-1500
        double totalanalysishistintegral = histanalysis->Integral();

        //Finds the # of 4L events in the fit range divided by total events in the analysis hist
        double analysis150bin = histanalysis->FindBin(150);
        double analysis1700bin = histanalysis->FindBin(1700);
        double eventanalysisfitrange = exp(log(histanalysis->Integral(analysis150bin, analysis1700bin)) - log(totalanalysishistintegral));
    
        //Calculates ratio between both fit integrals
        double fitratio = exp(log(acceptedfitintegral) - log(rangefitintegral));

        //Finds histogram fraction
        double histfraction = exp(log(fitratio) + log(eventanalysisfitrange));

        //Finds luminosity (from spreadsheet)
        double luminosity = 3000;

        //Finds efficiency
        double efficiency = exp(log(eventsanalysishist) - log(totaleventsran));

        //Finds crosssection (from spreadsheet)
        double crosssection = getCrossSection(); //Change this for different cross section

        //Finds background
        double backgroundest = exp(log(efficiency) + log(1000) + log(crosssection) + log(luminosity) + (2 * log(histfraction/2)));
        //For efficiency lose: log(1000) + log(crosssection) + log(luminosity)

        // double effnumb = exp(log(efficiency) + (2 * log(histfraction/2)));
        // outout << effnumb;
        // outout << "\n";

        //Prints non inclusive backgrounds
        if (getSumSwitch() == 0)
        {
            outout << backgroundest;
            outout << "\n";
        }

        //Adds my backgrounds if they need an inclusive at the end
        if (getSumSwitch() == 1)
        {
            g_sumOne = (g_sumOne + backgroundest);
        }
        if (getSumSwitch() == 2)
        {
            g_sumTwo = (g_sumTwo + backgroundest);
        }

        }
#endif