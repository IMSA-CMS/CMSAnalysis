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
std::vector<std::string> rowNames, std::vector<std::string> graphableTypes, std::vector<std::string> graphableTypes2, 
std::vector<TString> units, std::vector<std::string> units2, std::vector<std::string> channelNames,
std::vector<std::string> connecters, std::vector<std::string> connecters2, bool includeData)
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
	for (std::string connecter: connecters)
	{
		for(std::string dataType : graphableTypes) 
		{
			rowTitles.push_back(connecter + " " + dataType);
		}
	}

	// for (std::string connecter: connecters2)
	// {
	// 	for(std::string dataType : graphableTypes2) 
	// 	{
	// 		rowTitles.push_back(connecter + " " + dataType);
	// 	}
	// }

	std::vector<std::string> columnTitles;
	std::string entry = "";

	for (std::string connecter: connecters)
	{
		int unitCounter = 0;
		for(std::string dataType : graphableTypes) 
		{
			for(std::shared_ptr<Channel> channel : channels) 
			{ 	
				bool valid = false;
				for (std::string channelName : channelNames)
				{
					if (channelName == channel->getName())
					{
						valid = true;
						break;
					}
				}

				if (valid == false)
				{
					continue;
				}
				
				entry = "";
				TString xAxisName = units[unitCounter];
				TString yAxisName = "Events";
				dataName = Utility::removeSpaces(dataType);
				fileName = "jumboPlotStorage/" + Utility::removeSpaces("NoZVetoLumi") + "/" + channel->getName() + connecter + dataName + "DataMC.png";
				HistVariable fullDataType = HistVariable(connecter + dataType);
				bool includeSignal = false;
				TCanvas *canvas = plotFormatter->completePlot(analysis, fullDataType, xAxisName, yAxisName, false, includeSignal, includeData, channel->getName());
				canvas->SaveAs(fileName.c_str());
				plotFormatter->deleteHists();
				canvas->Close();
				delete canvas;
			
				entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";
				toAdd.push_back(entry);
			}
			unitCounter++;
			tableInput.push_back(toAdd);
			toAdd.clear();
		}
	}
	
	// for (std::string connecter: connecters2)
	// {
	// 	int unitCounter = 0;
	// 	for(std::string dataType : graphableTypes2) 
	// 	{
	// 		for(std::shared_ptr<Channel> channel : channels) 
	// 		{ 	
	// 			bool valid = false;
	// 			for (std::string channelName : channelNames)
	// 			{
	// 				if (channelName == channel->getName())
	// 				{
	// 					valid = true;
	// 					break;
	// 				}
	// 			}

	// 			if (valid == false)
	// 			{
	// 				continue;
	// 			}
				
	// 			entry = "";
	// 			TString xAxisName = units2[unitCounter];
	// 			TString yAxisName = "Events";
	// 			dataName = Utility::removeSpaces(dataType);
	// 			fileName = "jumboPlotStorage/" + Utility::removeSpaces("Higgs Signal") + "/" + channel->getName() + connecter + dataName + "DataMC.png";
	// 			HistVariable fullDataType = HistVariable(connecter + dataType);
	// 			bool includeSignal = false;
	// 			TCanvas *canvas = plotFormatter->completePlot(analysis, fullDataType, xAxisName, yAxisName, true, includeSignal, includeData, channel->getName());
	// 			canvas->SaveAs(fileName.c_str());
	// 			plotFormatter->deleteHists();
	// 			canvas->Close();
	// 			delete canvas;
			
	// 			entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";
	// 			toAdd.push_back(entry);
	// 		}
	// 		unitCounter++;
	// 		tableInput.push_back(toAdd);
	// 		toAdd.clear();
	// 	}
	// }
	
	auto tableData = std::make_shared<TableData>(tableInput, channelNames, rowTitles);
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

	// std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Signs"};
    // std::vector<std::string> graphableTypes = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
	// std::vector<TString> units = {"ETA", "DELTA R", "GEV", "RAD", "GEV/C"};


	// std::vector<std::string> channelNames = {"0.3"};

	//makePlots("Dark Photon Signal", DarkPhotonAnalysis, channels, rowNames, graphableTypes, units, channelNames);

	auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
	std::vector<std::shared_ptr<Channel>> higgsChannels = higgsAnalysis->getChannels();

	//std::vector<std::string> rowNames = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u"};
	std::vector<std::string> rowNames = {"ee", "e e", "eu", "e u", "uu", "u u"};
	std::vector<std::string> graphableTypes = {"Eta", "Pt"};
	std::vector<std::string> connecters = {"_1st Highest mu- ", "_1st Highest e- "}; 
	std::vector<TString> units = {"#eta", "GeV",};

	std::vector<std::string> graphableTypes2 = {"Same Sign Invariant Mass", "Opposite Sign Invariant Mass"};
	std::vector<std::string> connecters2 = {"_e- ", "_mu- "};
	std::vector<std::string> units2 = {" ", " "};

	//channelNames = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "eu", "e u", "uu", "u u"};
	std::vector<std::string> channelNames4 = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};
	std::vector<std::string> channelNames3 = {"eee", "eeu", "eue", "euu", "uue", "uuu"};
	std::vector<std::string> channelNames2 = {"ee", "e e", "eu", "e u", "uu", "u u"};

	//makePlots("Higgs Signal 4Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, units, units2, channelNames4, connecters, connecters2, false);
	//makePlots("Higgs Signal 3Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, units, units2, channelNames3, connecters, connecters2, false);
	makePlots("Higgs Signal 2Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, units, units2, channelNames2, connecters, connecters2, true);
}
