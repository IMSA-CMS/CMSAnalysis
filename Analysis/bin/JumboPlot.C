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
	std::vector<std::string> rowTitles;
	std::vector<std::string> columnTitles;
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
				entry = "";
				TString xAxisName = units[unitCounter];
				TString yAxisName = "Events";
				//toAdd.push_back(channel->getName());
				columnTitles.push_back(channel->getName());
				//toAdd.push_back(dataType);
				rowTitles.push_back(dataType);
				dataName = Utility::removeSpaces(dataType);
				fileName = "jumboPlotStorage/" + Utility::removeSpaces(signal) + "/" + channel->getName() + dataName + "DataMC.png";
				std::cout << "1" << std::endl;
				HistVariable fullDataType = HistVariable(dataType + " " + channel->getName(), "");
				std::cout << "Entering Plotformatter" << std::endl;
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
				unitCounter++;
			}
		}
	}

	auto tableData = std::make_shared<TableData>(tableInput, columnTitles, rowTitles);
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

	std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Signs"};
    std::vector<std::string> graphableTypes = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
	std::vector<TString> units = {"ETA", "DELTA R", "GEV", "RAD", "GEV/C"};


	std::vector<std::string> channelNames = {"0.3"};

	//makePlots("Dark Photon Signal", DarkPhotonAnalysis, channels, rowNames, graphableTypes, units, channelNames);

	auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
	std::vector<std::shared_ptr<Channel>> higgsChannels = higgsAnalysis->getChannels();

	//rowNames = {"ee", "u u"};
	//rowNames = {"ee", "e e", "eu", "e u", "uu", "u u"};
	rowNames = {"ee", "eu", "e u", "uu", "u u"};
    graphableTypes = {"Dxy", "Dz", "Eta", "Isolation", "Phi", "Pt"};
	units = {"Dxy [cm]", "Dz [cm]", "#eta", "Isolation", "#phi", "p_T [GeV/c]"};

	//channelNames = {"ee", "e e", "eu", "e u", "uu", "u u"};
	channelNames = {"ee", "eu", "e u", "uu", "u u"};
	//channelNames = {"ee", "u u"};

	makePlots("Higgs Signal", higgsAnalysis, higgsChannels, rowNames, graphableTypes, units, channelNames);
}
