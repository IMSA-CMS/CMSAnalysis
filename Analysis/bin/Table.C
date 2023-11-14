#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include "CMSAnalysis/Analysis/interface/TableWriter.hh"
#include "CMSAnalysis/Analysis/interface/TextTable.hh"
#include "CMSAnalysis/Analysis/interface/HTMLTable.hh"
#include "CMSAnalysis/Analysis/interface/LatexTable.hh"
#include "CMSAnalysis/Analysis/interface/PowerpointTable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
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

//Converts channel data to something TableData can work with
std::vector<std::vector<std::string>> makeTableInput(std::vector<std::vector<std::string>> oldInput, 
HistVariable dataType, std::shared_ptr<Channel> channel, double massTarget)
{
    std::vector<double> yields = channel->getYields(dataType.getName()); //error happens around here
    //probably an error with dataType.getName(), dataType is a histvariable so maybe the hist variable is wrong
    //find the right hist variable type
    std::vector<std::string> names = channel->getNames();

    std::vector<std::vector<std::string>> newInput = oldInput;

    std::vector<std::string> toAdd(3, "");
    int count = 0;
    for(std::string name : names) {
        toAdd.at(0) = names.at(count);
        toAdd.at(1) = channel->getName();
        toAdd.at(2) = roundDoubleString(yields.at(count), 4);
        newInput.push_back(toAdd);
        count++;
    }
    return newInput;
}

void Table() 
{
    std::vector<std::vector<std::string>> input;
    //Change what branching ratio particles you are looking at here
    std::vector<std::string> particles = {"e", "u"};
    std::vector<std::string> channels = Utility::channelMaker(particles, 4, true);
    //List massTargets here
    std::vector<double> massTargets = {900};
    //Change particle type here
    auto higgsAnalysis = std::make_shared<HiggsPlusPlusAnalysis>();
    //auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
    for(std::string channel : channels) {
        input.clear();
        for(double massTarget : massTargets) {
            //Change the histVariable to analyze different properties
            input = makeTableInput(input, HistVariable::SameSignMass("Cut4" + channel + channel + " Reco Invariant Mass Background"), higgsAnalysis->getChannel(channel + std::to_string((int) massTarget)), massTarget);
        }
        auto tableInput = std::make_shared<TableData>(input);
        //Change the type of table you want here
        auto table = std::make_shared<TextTable>();
        table->makeTable(tableInput, std::cout);
        std::cout << "\n" << std::endl;
    }
}
