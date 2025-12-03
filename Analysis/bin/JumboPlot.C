#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"


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


void makePlots(std::string signal, std::shared_ptr<FullAnalysis> analysis, std::vector<std::shared_ptr<Channel>> channels, 
std::vector<HistVariable> variables, bool includeData, bool includeSignal)
{
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Private Work (CMS Simulation/Data)");
	plotFormatter->setUpperMasslimit(2000);
	plotFormatter->setFirstBin(50);
	int graphSwitch = 5;

	double massTarget = 1400;

	//You don't have to change anything here, unless your y axis is something other than "Events"
	TCanvas* canvas;
	std::string fileName;
	int count = 0;
	std::vector<std::vector<std::string>> tableInput;
	std::vector<std::string> toAdd;
	std::vector<std::string> rowTitles;
	
	for (auto var : variables)
	{
		rowTitles.push_back(var.getName());
	}
	
	
	std::vector<std::string> columnTitles;
	std::string entry = "";
	//bool includeSignal = true;
	
	for (auto var: variables)
	{
//		int unitCounter = 0;

			for(std::shared_ptr<Channel> channel : channels) 
			{ 	
				columnTitles.push_back(channel->getName());
				
				entry = "";
				TString xAxisName = var.getUnit();//units[unitCounter];
				TString yAxisName = "Events";
				std::string storageRouting = (signal == "Dark Photons") ? "DarkPhoton" : "HiggsSignal";
				fileName = "jumboPlotStorage/" + storageRouting + "/" + channel->getName() + " " + var.getName() + " " + "DataMC.png";
				//HistVariable fullDataType = HistVariable(connecter + dataType);
				//bool includeSignal = false;
				TCanvas *canvas = plotFormatter->completePlot(analysis, var, xAxisName, yAxisName, false, includeSignal, includeData, channel->getName());
				if (canvas)
				{
					canvas->SaveAs(fileName.c_str());
					plotFormatter->deleteHists();
					canvas->Close();
					delete canvas;

					entry += "<img src=\"" + fileName + "\" alt=\"DataMC hist\" width=\"100%\" height = \"80%\">";
				}
				else
				{
					entry += "<p>No histogram available</p>";
				}
				toAdd.push_back(entry);

			}
//			unitCounter++;
			tableInput.push_back(toAdd);
			toAdd.clear();
		
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
	ParticleType::loadParticleDatabase("../../Utility/interface/ParticleData.txt");

	////////////////////////////////// HIGGS ANALYSIS //////////////////////////////////
	std::string particleDatabase(Utility::getFullPath("ParticleData.txt"));
	auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
	std::vector<std::shared_ptr<Channel>> higgsChannels = higgsAnalysis->getChannels();

	std::vector<std::shared_ptr<Channel>> channelTwoLeptons;
	std::vector<std::shared_ptr<Channel>> channelThreeLeptons;
	std::vector<std::shared_ptr<Channel>> channelFourLeptons;
	for (auto channel : higgsChannels)
	{
		std::string name = channel->getName().substr(0, 4);
		int count = std::count(name.begin(), name.end(),'_');

		if (channel->getName() == "none")
		{
			continue;
		}
		if (count == 0)
		{
			channelFourLeptons.push_back(channel);
		}
		else if (count == 2)
		{
			channelTwoLeptons.push_back(channel);
		}
		else if (count == 1)
		{
			channelThreeLeptons.push_back(channel);
		}
	}

	// //channelNames = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "eu", "e u", "uu", "u u"};
	std::vector<std::string> channelNames4 = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};
	std::vector<std::string> channelNames3 = {"eee", "eeu", "eue", "euu", "uue", "uuu"};
	std::vector<std::string> channelNames2 = {"ee", "e e", "eu", "e u", "uu", "u u"};

	// makePlots("Higgs Signal 4Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, graphableTypes3, units, units2, units3, channelNames4, connecters, connecters2, connecters3, false);
	// makePlots("Higgs Signal 3Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, graphableTypes3, units, units2, units3, channelNames3, connecters, connecters2, connecters3, false);
	// makePlots("Higgs Signal 2Channel", higgsAnalysis, higgsChannels, rowNames, graphableTypes, graphableTypes2, graphableTypes3, units, units2, units3, channelNames2, connecters, connecters2, connecters3, true);

	std::vector<HistVariable> variables = {
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Eta),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Eta),
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Phi),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Phi),
		HistVariable(ParticleType::electron(), 0, HistVariable::VariableType::SameSignInvariantMass),
		HistVariable(ParticleType::muon(), 0, HistVariable::VariableType::SameSignInvariantMass),
		HistVariable(ParticleType::electron(), 0, HistVariable::VariableType::OppositeSignInvariantMass),
		HistVariable(ParticleType::muon(), 0, HistVariable::VariableType::OppositeSignInvariantMass),
		HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
		HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),

		HistVariable(HistVariable::VariableType::RecoOppositeSignInvariantMass),
		HistVariable(HistVariable::VariableType::RecoOppositeSignInvariantMass),
		HistVariable(HistVariable::VariableType::RecoSameSignInvariantMass),
		HistVariable(HistVariable::VariableType::RecoSameSignInvariantMass),

		HistVariable(HistVariable::VariableType::MET),
	};

	auto darkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>();
		std::vector<HistVariable> darkphotonVariables = {
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Eta),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Phi),
		HistVariable(ParticleType::leptonJet(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::leptonJet(), 1, HistVariable::VariableType::Eta),
		};

		std::vector<std::shared_ptr<Channel>> darkPhotonChannels = darkPhotonAnalysis->getChannels();
		std::vector<std::shared_ptr<Channel>> controlChannels;
		std::vector<std::shared_ptr<Channel>> signalChannels;
		for (auto channel : darkPhotonChannels)
		{
			if (channel->getName().find("Low Mass and Different Sign") == std::string::npos)
			{
				controlChannels.push_back(channel);
			}
			else
			{
				signalChannels.push_back(channel);
			}
		}
		// std::vector<std::string> darkPhotonControlChannels = {
		// 	"High Mass and Same Sign/1jet",
		// 	"Low Mass and Same Sign/1jet",
		// 	"High Mass and Different Sign/1jet",
		// 	"High Mass and Same Sign/multiJet",
		// 	"Low Mass and Same Sign/multiJet",
		// 	"High Mass and Different Sign/multiJet"
		// };
	
	////////////////////////////////// DP ANALYSIS //////////////////////////////////
	
	// const std::string output_cuts_files1_10 = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/LeptonJetReconstruction_All/";
	// const std::string cross_section = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt";
	//  auto darkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>();
	//  std::vector<std::shared_ptr<Channel>> channels = darkPhotonAnalysis->getChannels();

	// std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Signs"};
    // std::vector<std::string> graphableTypes = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
	// std::vector<TString> units = {"ETA", "DELTA R", "GEV", "RAD", "GEV/C"};
	// std::vector<std::string> channelNames = {"0.3"};

	// std::vector<std::string> bsvtfutsps = {}; // bullshitvectortofillupthesestupidparameterslots
	// std::vector<std::string> connectors = {"_1st Highest Lepton Jet "};

	auto darkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>(); 
	std::vector<std::shared_ptr<Channel>> darkPhotonChannels = darkPhotonAnalysis->getChannels();

	std::vector<HistVariable> darkPhotonVariables = 
	{
		HistVariable(HistVariable::VariableType::DarkPhotonBDTOutput),
		HistVariable(HistVariable::VariableType::LeptonJetMass),
		HistVariable(HistVariable::VariableType::LeptonJetDeltaR),
		HistVariable(HistVariable::VariableType::MET),
		HistVariable(HistVariable::VariableType::Eta),
		HistVariable(HistVariable::VariableType::Pt),
		HistVariable(HistVariable::VariableType::Phi),
		
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Eta),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Eta),
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Pt),
		HistVariable(ParticleType::electron(), 1, HistVariable::VariableType::Phi),
		HistVariable(ParticleType::muon(), 1, HistVariable::VariableType::Phi),
	};

	// makePlots("Higgs Signal 2Channel", higgsAnalysis, channelTwoLeptons, variables ,true, false);
	// makePlots("Higgs Signal 3Channel", higgsAnalysis, channelThreeLeptons, variables , false, true);
	// makePlots("Higgs Signal 4Channel", higgsAnalysis, channelFourLeptons, variables , false, true);

	makePlots("Dark Photons", darkPhotonAnalysis, darkPhotonChannels, darkPhotonVariables, false, false);
	
}
//std::string signal, std::shared_ptr<FullAnalysis> analysis, std::vector<std::shared_ptr<Channel>> channels, std::vector<HistVariable> variables, bool includeData, bool includeSignal