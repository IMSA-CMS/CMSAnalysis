#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsGenComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCutsAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/DYComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/HTMLTable.hh"
#include "CMSAnalysis/Analysis/interface/SimpleAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/SimpleHiggsComparison.hh"
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
#include <string>
#include "CMSAnalysis/DataCollection/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/SimpleChannel.hh"

void JumboPlot() {
	//Change things in this section

	//Put your analysises here. If you only have a simpleAnalysis or only superimposed histograms that's ok, just leave the others at default
	auto higgsStackAnalysis = std::make_shared<HiggsCutsAnalysis>();
	auto higgsSuperImpAnalysis = std::make_shared<HiggsCutsAnalysis>();
	auto simpleAnalysis = std::make_shared<SimpleHiggsComparison>();
	//Extra text is the second parameter
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "");
	std::vector<std::shared_ptr<Channel>> stackChannels = higgsStackAnalysis->getChannels();
	std::vector<std::shared_ptr<Channel>> superImpChannels = higgsSuperImpAnalysis->getChannels();
	//Controls what graph types to make. 1 is stacked only, 2 is superimposed only, 3 is both. 4 is for SimpleAnalysis only
	int graphSwitch = 2;
	//Put all variables you can graph here
	//Choices are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
	std::vector<std::string> graphableTypes = {"Same Sign Inv Mass"};
	//Change this to whatever process your signal is
	std::string signalName = "Higgs Signal";
	TString units = " [GEV]";



	//You don't have to change anything here, unless your y axis is something other than Events
	TCanvas* canvas;
	std::string fileName;
	std::string dataName; 
	int count = 0;
	std::vector<std::vector<std::string>> tableInput;
	std::vector<std::string> toAdd;
	std::string entry = "";
	if(graphSwitch != 4) {
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
			for(std::string dataType : graphableTypes) {
				entry = "";
				TString xAxisName = dataType.c_str() + units;
				TString yAxisName = "Events";
				TString superyAxisName = "Events (1/Integral)";
				
				toAdd.push_back(dataType);
				toAdd.push_back(channelName);
				dataName = Utility::removeSpaces(dataType);
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
					canvas = plotFormatter->superImposedHist(superImpChannels.at(count), dataType, xAxisName, superyAxisName);
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
	}
	else {
		for(std::shared_ptr<SimpleChannel> channel : simpleAnalysis->getChannels()) {
			std::string channelName = channel->getName();
			for(std::string dataType : graphableTypes) {
				entry = "";
				TString xAxisName = dataType.c_str() + units;
				TString yAxisName = "Events (1/Integral)";
			
				toAdd.push_back(dataType);
				toAdd.push_back(channelName);
				
				dataName = Utility::removeSpaces(dataType);

				fileName = "jumboPlotStorage/" + channelName + dataName + ".png";
				canvas = plotFormatter->simpleAnalysisHist(channel->getHists(dataType), channel->getNames(), xAxisName, yAxisName);
				canvas->SaveAs(fileName.c_str());
				plotFormatter->deleteHists();
				canvas->Close();
				delete canvas;
				entry += "<img src=\"" + fileName + "\" alt=\"superImposed hist\" width=\"100%\" height = \"80%\">";
				toAdd.push_back(entry);
				tableInput.push_back(toAdd);
				toAdd.clear();
			}
		}
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
