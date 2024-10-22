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
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
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
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/SimpleChannel.hh"

#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"


template <typename AnalysisType>
void makePlots(std::string signal, std::shared_ptr<AnalysisType> analysis, std::vector<std::shared_ptr<Channel>> channels, 
std::vector<std::string> rowNames, std::vector<std::string> graphableTypes, std::vector<TString> units, std::vector<std::string> channelNames)
{
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Private Work (CMS Simulation/Data)");
	plotFormatter->setUpperMasslimit(2000);
	plotFormatter->setNumBins(5);
	plotFormatter->setFirstBin(50);

	int graphSwitch = 5;

	double massTarget = 1400;

	//You don't have to change anything here, unless your y axis is something other than "Events"
	TCanvas* canvas;
	std::string fileName;
	std::string dataName; 
	int count = 0;
	std::vector<std::vector<std::string>> tableInput;
	std::vector<std::string> toAdd;
	std::string entry = "";

	if(graphSwitch == 5) {
		for(std::shared_ptr<Channel> channel : channels) { 
			
			bool valid = false;
			for (std::string channelName : channelNames)
			{
				if (channelName == channel->getName())
				{
					valid = true;
				}
			}
			if (valid == false)
			{
				continue;
			}

			for(std::string processName : channel->getNames()) {
				//std::cout << processName << std::endl;
				//Change this line to make the described name your signal process name.
				if(processName == signal) {
					channel->labelProcess("signal", processName);
				}
				// "Monte Carlo Data"
				else if(processName == "Data") { //This line is only used for complete plots
					channel->labelProcess("data", processName);
				}
				else {
					channel->labelProcess("background", processName);
				}
			}

			std::cout << "Background size: " << channel->getNamesWithLabel("background").size() << std::endl;

			std::string channelName = channel->getName();
			//toAdd.push_back(channelName);
			int unitCounter = 0;
			for(std::string dataType : graphableTypes) {
				for (std::string rowName : rowNames) {
					
					std::cout << "HERERERERERERERERERE" << std::endl;

					entry = "";
					//TString xAxisName = "OSDL " + units[unitCounter];
					TString xAxisName = units[unitCounter];
					TString yAxisName = "Events";
					toAdd.push_back(rowName);
					toAdd.push_back(dataType);
					//toAdd.push_back(rowName);
					dataName = Utility::removeSpaces(dataType);
					fileName = "jumboPlotStorage/" + Utility::removeSpaces(signal) + "/" + Utility::removeSpaces(newRowName) + dataName + "DataMC.png";
					//fileName = channelName + dataName + "DataMC.png";
					std::string fullDataType = dataType + " " + rowName;
					TCanvas *canvas = plotFormatter->completePlot(analysis, fullDataType, xAxisName, yAxisName, true, false, channel->getName());
					//TCanvas *canvas = plotFormatter->completePlot(higgsAnalysis, "Invariant Mass", xAxisName, yAxisName, true, channelName);
					canvas->SaveAs(fileName.c_str());
					plotFormatter->deleteHists();
					canvas->Close();
					delete canvas;
				
					entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";

					toAdd.push_back(entry);
					tableInput.push_back(toAdd);
					toAdd.clear();
				}
				unitCounter++;
			}
		}
	}

	auto tableData = std::make_shared<TableData>(tableInput);
	auto table = std::make_shared<HTMLTable>();
	std::ofstream htmlFile;
	htmlFile.open(Utility::removeSpaces(signal) + ".html");
	htmlFile << "<!DOCTYPE html>" << std::endl;
	htmlFile << "<html>" << std::endl;
	htmlFile << "<body>" << std::endl;
	table->makeTable(tableData, htmlFile);
	htmlFile << "</body>" << std::endl;
	htmlFile << "</html>" << std::endl;
	htmlFile.close();
	tableInput.clear();
}

void JumboPlot()
{
	//auto DarkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>();
	//std::vector<std::shared_ptr<Channel>> channels = DarkPhotonAnalysis->getChannels();

	//std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Signs"};
    //std::vector<std::string> graphableTypes = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
	//std::vector<TString> units = {"ETA", "DELTA R", "GEV", "RAD", "GEV/C"};


	//std::string channelName = "0.3";

	//makePlots("Dark Photon Signal", DarkPhotonAnalysis, channels, rowNames, graphableTypes, units, channelName);

	std::vector<std::string> channelNames = {"0.3"};
	std::vector<std::shared_ptr<Channel>> higgsChannels = higgsAnalysis->getChannels();

	//rowNames = {"u u"};
	//rowNames = {"ee", "e e", "eu", "e u", "uu", "u u"};
	rowNames = {"ee", "eu", "e u", "uu", "u u"};
    graphableTypes = {"Dxy", "Dz", "Eta", "Isolation", "Phi", "Pt"};
	units = {"", "", "ETA", "", "RAD", "GEV/C"};

	//channelNames = {"ee", "e e", "eu", "e u", "uu", "u u"};
	channelNames = {"ee", "eu", "e u", "uu", "u u"};

	makePlots("Higgs Signal", higgsAnalysis, higgsChannels, rowNames, graphableTypes, units, channelNames);
}

void Temp() { //JumboPlot()
	//Change things in this section

	//Put your analysises here. If you only have a simpleAnalysis or only superimposed histograms that's ok, just leave the others at default
	//auto higgsStackAnalysis = std::make_shared<HiggsCutsAnalysis>();
	//auto higgsSuperImpAnalysis = std::make_shared<HiggsCutsAnalysis>();
	//auto simpleAnalysis = std::make_shared<SimpleHiggsComparison>();
	//auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
	auto DarkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>();
	//Extra text is the second parameter
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Private Work (CMS Simulation/Data)");
	plotFormatter->setUpperMasslimit(2000);
	plotFormatter->setNumBins(5);
	plotFormatter->setFirstBin(50);
	//std::vector<std::shared_ptr<Channel>> stackChannels = higgsStackAnalysis->getChannels();
	//std::vector<std::shared_ptr<Channel>> superImpChannels = higgsSuperImpAnalysis->getChannels(); 
	//std::vector<std::shared_ptr<Channel>> higgsChannels = higgsAnalysis->getChannels();
	std::vector<std::shared_ptr<Channel>> channels = DarkPhotonAnalysis->getChannels();
	//Controls what graph types to make. 1 is stacked only, 2 is superimposed only, 3 is both, 4 is for SimpleAnalysis only, and 5 is for higgsAnalysis.
	int graphSwitch = 5;
	//Put all variables you can graph here
	//Choices are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)

	std::vector<std::string> signalTypes = {"Dark Photon Signal"};

	std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Signs"};
    std::vector<std::string> graphableTypes = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
	
	//Change this to whatever process your signal is
	//std::string signalName = "Higgs Signal";
	//TString units = " [GEV]";
	std::vector<TString> units = {"ETA", "DELTA R", "GEV", "RAD", "GEV/C"};
	

	double massTarget = 1400;

	//You don't have to change anything here, unless your y axis is something other than "Events"
	TCanvas* canvas;
	std::string fileName;
	std::string dataName; 
	int count = 0;
	std::vector<std::vector<std::string>> tableInput;
	std::vector<std::string> toAdd;
	std::string entry = "";
	for (std::string signal : signalTypes)
	{
		if(graphSwitch == 5) {
			for(std::shared_ptr<Channel> channel : channels) { 
				for(std::string processName : channel->getNames()) {
					
					//std::cout << processName << std::endl;
					//Change this line to make the described name your signal process name.
					if(processName == signal) {
						channel->labelProcess("signal", processName);
					}
					// "Monte Carlo Data"
					else if(processName == "Data") { //This line is only used for complete plots
						channel->labelProcess("data", processName);
					}
					else {
						channel->labelProcess("background", processName);
					}
					
					/*
					if(processName == "Higgs Signal") {
						channel->labelProcess("signal", processName);
					}
					// "Monte Carlo Data"
					else if(processName == "Data") { //This line is only used for complete plots
						channel->labelProcess("data", processName);
					}
					else {
						channel->labelProcess("background", processName);
					}
					*/
				}
				std::string channelName = channel->getName();
				//toAdd.push_back(channelName);
				int unitCounter = 0;
				for(std::string dataType : graphableTypes) {
					for (std::string rowName : rowNames) {
						entry = "";
						//TString xAxisName = "OSDL " + units[unitCounter];
						TString xAxisName = units[unitCounter];
						TString yAxisName = "Events";
						toAdd.push_back(dataType);
						toAdd.push_back(rowName);
						dataName = Utility::removeSpaces(dataType);
						fileName = "jumboPlotStorage/" + Utility::removeSpaces(signal) + "/" + Utility::removeSpaces(rowName) + dataName + "DataMC.png";
						//fileName = channelName + dataName + "DataMC.png";
						std::string fullDataType = dataType + " " + rowName;
						TCanvas *canvas = plotFormatter->completePlot(DarkPhotonAnalysis, fullDataType, xAxisName, yAxisName, true, false, "0.3");
						//TCanvas *canvas = plotFormatter->completePlot(higgsAnalysis, "Invariant Mass", xAxisName, yAxisName, true, channelName);
						canvas->SaveAs(fileName.c_str());
						plotFormatter->deleteHists();
						canvas->Close();
						delete canvas;
					
						entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";
				
						toAdd.push_back(entry);
						tableInput.push_back(toAdd);
						toAdd.clear();
					}
					unitCounter++;
				}
			}
		}

		auto tableData = std::make_shared<TableData>(tableInput);
		auto table = std::make_shared<HTMLTable>();
		std::ofstream htmlFile;
		htmlFile.open(Utility::removeSpaces(signal) + ".html");
		htmlFile << "<!DOCTYPE html>" << std::endl;
		htmlFile << "<html>" << std::endl;
		htmlFile << "<body>" << std::endl;
		table->makeTable(tableData, htmlFile);
		htmlFile << "</body>" << std::endl;
		htmlFile << "</html>" << std::endl;
		htmlFile.close();
		tableInput.clear();
	}
	/*
	if(graphSwitch == 5) {
		for(std::shared_ptr<Channel> channel : channels) { 
			for(std::string processName : channel->getNames()) {
				
				//std::cout << processName << std::endl;
				//Change this line to make the described name your signal process name.
				if(processName == "Dark Photon Signal") {
					channel->labelProcess("signal", processName);
				}
				// "Monte Carlo Data"
				else if(processName == "Data") { //This line is only used for complete plots
					channel->labelProcess("data", processName);
				}
				else {
					channel->labelProcess("background", processName);
				}
				
			}
			std::string channelName = channel->getName();
			//toAdd.push_back(channelName);
			int unitCounter = 0;
			for(std::string dataType : graphableTypes) {
				for (std::string rowName : rowNames) {
					entry = "";
					//TString xAxisName = "OSDL " + units[unitCounter];
					TString xAxisName = units[unitCounter];
					TString yAxisName = "Events";
				
					toAdd.push_back(dataType);
					toAdd.push_back(rowName);
					dataName = Utility::removeSpaces(dataType);
					fileName = "jumboPlotStorage/" + Utility::removeSpaces(rowName) + dataName + "DataMC.png";
					//fileName = channelName + dataName + "DataMC.png";
					std::string fullDataType = dataType + " " + rowName;
					TCanvas *canvas = plotFormatter->completePlot(DarkPhotonAnalysis, fullDataType, xAxisName, yAxisName, true, false, "0.3");
					//TCanvas *canvas = plotFormatter->completePlot(higgsAnalysis, "Invariant Mass", xAxisName, yAxisName, true, channelName);
					canvas->SaveAs(fileName.c_str());
					plotFormatter->deleteHists();
					canvas->Close();
					delete canvas;
				
					entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";
			
					toAdd.push_back(entry);
					tableInput.push_back(toAdd);
					toAdd.clear();
				}
				unitCounter++;
				
			}
		}
	}
	*/


/*
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
	*/
/*
	auto tableData = std::make_shared<TableData>(tableInput);
	auto table = std::make_shared<HTMLTable>();
	std::ofstream htmlFile;
	htmlFile.open("DarkPhoton.html");
	htmlFile << "<!DOCTYPE html>" << std::endl;
	htmlFile << "<html>" << std::endl;
	htmlFile << "<body>" << std::endl;
	table->makeTable(tableData, htmlFile);
	htmlFile << "</body>" << std::endl;
	htmlFile << "</html>" << std::endl;
	htmlFile.close();
*/
}

