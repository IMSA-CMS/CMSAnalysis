#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include "CMSAnalysis/Analysis/interface/HTMLTable.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include <vector>
#include <map>
#include "CMSAnalysis/DataCollection/interface/Utility.hh"

void JumboPlot() {
	auto higgsStackAnalysis = std::make_shared<HiggsComparisonAnalysis>();
	auto higgsSuperImpAnalysis = std::make_shared<HiggsComparisonAnalysis>();
	//Extra text is the second parameter
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "");
	std::vector<std::shared_ptr<Channel>> stackChannels = higgsStackAnalysis->getChannels();
	std::vector<std::shared_ptr<Channel>> superImpChannels = higgsSuperImpAnalysis->getChannels();
	//Controls what graph types to make. 1 is stacked only, 2 is superimposed only, 3 is both.
	int graphSwitch = 2;
	//Put all variables you can graph here
	std::vector<HistVariable> graphableTypes = {HistVariable::InvariantMass, HistVariable::SameSignMass, HistVariable::pT, HistVariable::MET};	
	std::map<HistVariable, std::string> histVariableNames;
	histVariableNames[HistVariable::InvariantMass] = "Invariant Mass";
	histVariableNames[HistVariable::SameSignMass] = "Same Sign Inv Mass";
	histVariableNames[HistVariable::pT] = "Leading Lept pT";
	histVariableNames[HistVariable::MET] = "MET";
	//Change this to whatever process your signal is
	std::string signalName = "Old Higgs";
	TCanvas* canvas;
	std::string fileName;
	std::string dataName; 
	int count = 0;
	std::vector<std::vector<std::string>> tableInput;
	std::vector<std::string> toAdd;
	std::string entry = "";
	for(std::shared_ptr<Channel> channel : stackChannels) {
		for(std::string processName : channel->getNames()) {
			//Change this to your signal process name
			if(processName != signalName) {
				channel->labelProcess("background", processName);
			}
			else {
				channel->labelProcess("signal", processName);
			}
		}
		for(std::string processName : superImpChannels.at(count)->getNames()) {
            //Change this to your signal process name
            if(processName != signalName) {
                superImpChannels.at(count)->labelProcess("background", processName);
            }
            else {
		        superImpChannels.at(count)->labelProcess("signal", processName);
            }
        }
		std::string channelName = channel->getName();
		//toAdd.push_back(channelName);
		for(HistVariable dataType : graphableTypes) {
			entry = "";
			TString xAxisName = histVariableNames.find(dataType)->second + " [GEV]";
			TString yAxisName = "Events";
			
			toAdd.push_back(histVariableNames.find(dataType)->second);
			toAdd.push_back(channelName);
			
			dataName = Utility::removeSpaces(histVariableNames.find(dataType)->second);
			if(graphSwitch == 1 || graphSwitch == 3) {
				fileName = "jumboPlotStorage/" + channelName + dataName + "Stack.png";
				canvas = plotFormatter->superImposedStackHist(channel, dataType, xAxisName, yAxisName);
				canvas->SaveAs(fileName.c_str());
				plotFormatter->deleteHists();
				canvas->Close();
				delete canvas;
				if(graphSwitch == 1) {
					entry += "<img src=\"" + fileName + "\" alt=\"stack hist\" width=\"100%\" height = \"80%\">";
				}
				else {
					entry += "<img src=\"" + fileName + "\" alt=\"stack hist\" width=\"50%\" height = \"40%\">";
				}
			}

			if(graphSwitch == 2 || graphSwitch == 3) {
				fileName = "jumboPlotStorage/" + channelName + dataName + ".png";
				canvas = plotFormatter->superImposedHist(superImpChannels.at(count), dataType, true, xAxisName, yAxisName);
				canvas->SaveAs(fileName.c_str());
				plotFormatter->deleteHists();
				canvas->Close();
				delete canvas;
				if(graphSwitch == 2) {
					entry += "<img src=\"" + fileName + "\" alt=\"superImposed hist\" width=\"100%\" height = \"80%\">";
				}
				else {
					entry += "<img src=\"" + fileName + "\" alt=\"superImposed hist\" width=\"50%\" height = \"40%\">";
				}
			}

			toAdd.push_back(entry);
			tableInput.push_back(toAdd);
			toAdd.clear();
		}
		count++;
	}
	auto tableData = std::make_shared<TableData>(tableInput);
	auto table = std::make_shared<HTMLTable>();
	std::ofstream htmlFile;
	htmlFile.open("index.html");
	htmlFile << "<!DOCTYPE html>" << std::endl;
	htmlFile << "<html>" << std::endl;
	htmlFile << "<body>" << std::endl;
	table->makeTable(tableData, htmlFile);
	htmlFile << "</body>" << std::endl;
	htmlFile << "</html>" << std::endl;
	htmlFile.close();
}