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
    std::vector<std::string> channels =  {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
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
        HistVariable histVariable("Same Sign Invariant Mass");
        std::shared_ptr<Channel> channelPtr = higgsAnalysis->getChannel(channel);
   
        //Gets names of decays in channel and their corresponding yields by index (i.e yields[i] corresponds with names[i])
       std::vector<double> yields = channelPtr->getYields(histVariable); 
        std::vector<std::string> names = channelPtr->getNames();

        // WWZ Boson Extraction
        auto channelProcesses = channelPtr->getProcesses();
        

        for (int stringIndex = names.size() - 1; stringIndex >= 0; --stringIndex) {
            std::string name = names[stringIndex];
            if (name.find("Higgs signal") != string::npos) {
                names.erase(names.begin() + stringIndex);
                yields.erase(yields.begin() + stringIndex);
            }
        }
        
        for (auto process : channelProcesses)
         {
            if (process->getName() != "t#bar{t}, Multiboson Background") continue;
            auto singleProcess =  process->getSingleProcess("wzto3lnu");
            double yield = singleProcess.getExpectedYield(histVariable);
            
            yields.push_back(yield);
        }
   


        // Converts yields to strings and stores in row for final table
        std::vector<std::string> dataRow;
        for (double yield : yields) dataRow.push_back(roundDoubleString(yield, 4));

        //Adds yields to final decay yield data grid
        finalTableData.push_back(dataRow);

        //Saves truncated names to be used as columnNames in data table.
        columnNames = names;
        columnNames.push_back("wz");
       
    }
    
    


   


    

    

    auto tableInput = std::make_shared<TableData>(finalTableData, columnNames, channelNames);
    //Change the type of table you want here
    auto table = std::make_shared<TextTable>();
    table->makeTable(tableInput, std::cout);
    std::cout << "\n" << std::endl;
 
}
