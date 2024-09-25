#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/TableWriter.hh"
#include "CMSAnalysis/Utility/interface/TextTable.hh"
#include "CMSAnalysis/Utility/interface/HTMLTable.hh"
#include "CMSAnalysis/Utility/interface/LatexTable.hh"
#include "CMSAnalysis/Utility/interface/PowerpointTable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include "CMSAnalysis/Utility/interface/Utility.hh"

//Rounds doubles when converted to strings to get rid of trailing zeroes
std::string roundDoubleString(double doub, int digits) 
{
    std::ostringstream rounder;
    rounder << std::setprecision(digits);
    rounder << doub;
    std::string out = rounder.str();
    return out;
}



void Table() 
{
    //Channel Configurations
    std::vector<std::string> particles = {"e", "u"}; // Chosen branch ratio particles
    std::vector<std::string> channels = Utility::channelMaker(particles, 4, true); 

    //make sure these match with massTargets in HiggsCompleteAnaylsis
    std::vector<double> massTargets { 500, 600, 700, 800, 900, 1100, 1200, 1300, 1400};

    //Toggle for Analysis type
    //auto higgsAnalysis = std::make_shared<HiggsPlusPlusAnalysis>();
    auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();


    //Desired Background Decay Types
    std::vector<std::string> backgroundTypes
    {
            "t#bar{t}, WW, WZ, ZZ Background",
            "Drell-Yan Background",
            "QCD Background",
            "ZZ Background"
    };

    int backgroundTypeCount = sizeof(backgroundTypes)/sizeof(backgroundTypes[0]);
    
    
    //Finalized table data for each channel and decay yields 
    std::vector<std::vector<std::string>> finalTableData;
    std::vector<std::string> columnNames;
    std::vector<std::string> channelNames;
    
    

    // Loop through all channels and store yields into finalTableData in order of associated decay names
    for (std::string channel : channels) 
    {
        //skips none channel
        if (channel == "none") continue;
        channelNames.push_back(channel);
        
        //gets sameSignMass hist data
        HistVariable histVariable = HistVariable::sameSignMass(channel + "__hists/" + channel + "_1st Highest mu- Dxy");
        std::shared_ptr<Channel> channelPtr = higgsAnalysis->getChannel(channel);
   
        //Gets names of decays in channel and their corresponding yields by index (i.e yields[i] corresponds with names[i])
        std::vector<double> yields = channelPtr->getYields(histVariable); //error happens around here
        std::vector<std::string> names = channelPtr->getNames();

        //Remove All names and yields that are not associated with this channel (Leaves background decay and signal mass yields)
        for (int nameIndex = names.size() - 1  ; nameIndex >= 0;--nameIndex ) {
            std::string name = names[nameIndex];
            if (find(backgroundTypes.begin(), backgroundTypes.end(), name) == backgroundTypes.end() && name.find(channel) == string::npos ) {
                names.erase(names.begin()+ nameIndex);
                yields.erase(yields.begin()+ nameIndex);
            }
        }
        
        //Converts yields to strings and stores in row for final table
        std::vector<std::string> dataRow;
        for (double yield : yields) dataRow.push_back(roundDoubleString(yield, 4));

        //Adds yields to final decay yield data grid
        finalTableData.push_back(dataRow);

        //Saves truncated names to be used as columnNames in data table.
        columnNames = names;
    }
    
    
    // Renames column to get rid of channel specificity (i.e removes uuuu, eeee, eueu)
    for (int nameIndex = 0; nameIndex < columnNames.size(); ++nameIndex) {
        std::string name = columnNames[nameIndex];
        if (name.find("Higgs signal") != string::npos) {
           int pos = name.find("_");
            columnNames[nameIndex] = "Higgs Signal: " + name.substr(pos+5) + " GeV";
        }
    }

   


    

    

    auto tableInput = std::make_shared<TableData>(finalTableData, columnNames, channelNames);
    //Change the type of table you want here
    auto table = std::make_shared<TextTable>();
    table->makeTable(tableInput, std::cout);
    std::cout << "\n" << std::endl;
 
}
