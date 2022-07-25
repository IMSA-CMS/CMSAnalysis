#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include "CMSAnalysis/Analysis/interface/TableWriter.hh"
#include "CMSAnalysis/Analysis/interface/TextTable.hh"
#include "CMSAnalysis/Analysis/interface/HTMLTable.hh"
#include "CMSAnalysis/Analysis/interface/LatexTable.hh"
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

std::string roundDoubleString(std::string doub, int digits) 
{
    bool doneCutting = false;
	//Removes zeroes that happen when converting double to string
	for(int index = doub.length() - 1; index >= 0; index--) {
		if((doub.at(index) == '0' || doub.at(index) == '.') && doneCutting == false) {
			if(doub.at(index) == '.') {
				doneCutting = true;
			}
			doub.pop_back();
		}
		else {
			doneCutting = true;
		}
	}
    std::string out = doub;
    /*std::stringstream rounder;
    rounder << std::setprecision(digits);
    rounder << doub;
    std::string out = rounder.str() */
    return out;
}

std::vector<std::vector<std::string>> makeTableInput(std::vector<std::vector<std::string>> oldInput, std::shared_ptr<Channel> channel, double massTarget)
{
    std::vector<double> yields = channel->getYields();
    std::vector<std::string> names = channel->getNames();
    std::vector<std::vector<std::string>> newInput = oldInput;
    std::vector<std::string> toAdd(3, "");
    int count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < names.size())
    {
        toAdd.at(0) = names.at(count);
        toAdd.at(1) = std::to_string((int) massTarget);
        toAdd.at(2) = roundDoubleString(std::to_string(yields.at(count)), 2);
        newInput.push_back(toAdd);
        count++;
    }
    return newInput;
}

void Table() 
{
    double massTarget = 700;
    std::shared_ptr<Channel> leptonBackgrounds = ChannelLoader::makeChannel("default", massTarget);
    std::vector<std::vector<std::string>> input;
    input = makeTableInput(input, leptonBackgrounds, massTarget);
    massTarget = 900;
    std::shared_ptr<Channel> leptonBackgrounds2 = ChannelLoader::makeChannel("default", massTarget);
    input = makeTableInput(input, leptonBackgrounds2, massTarget);
    auto tableInput = std::make_shared<TableData>(input);
    auto table = std::make_shared<LatexTable>();
    table->makeTable(tableInput, std::cout);
}