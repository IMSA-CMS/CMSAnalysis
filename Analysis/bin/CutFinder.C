#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "TH1.h"
#include <memory>
#include <vector>
#include <map>
#include <cmath>

double calculateEvents(std::vector<double> crossSections, std::vector<TH1*> hists, std::vector<int> generatedEvents, double luminosity, int cut) {
    double selected = 0;
    int count = 0;
    for(auto hist : hists) {
        selected += crossSections.at(count) * luminosity * (hist->Integral(cut, hist->GetNbinsX()) / (double) generatedEvents.at(count));
        count++;
    }
    return selected;
}

void CutFinder() {
    auto analysis = std::make_shared<HiggsPlusPlusAnalysis>();
    std::vector<HistVariable> cutTypes = {HistVariable::InvariantMass, HistVariable::SameSignMass, HistVariable::pT, HistVariable::MET};
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    int luminosity = 3000;
    //Change this to your signal process name
    std::string signalName = "Higgs Signal";


    std::map<HistVariable, std::string> histVariableNames;
	histVariableNames[HistVariable::InvariantMass] = "Invariant Mass";
	histVariableNames[HistVariable::SameSignMass] = "Same Sign Inv Mass";
	histVariableNames[HistVariable::pT] = "Leading Lept pT";
	histVariableNames[HistVariable::MET] = "MET";
	histVariableNames[HistVariable::GenSimSameSignMass] = "GenSim Same Sign Inv Mass";
	histVariableNames[HistVariable::GenSimPt] = "GenSim Leading Lepton pT";	
    for(std::shared_ptr<Channel> channel : channels) {
		for(std::string processName : channel->getNames()) {
			if(processName != signalName) {
				channel->labelProcess("background", processName);
			}
			else {
				channel->labelProcess("signal", processName);
			}
		}
    }
    for(HistVariable dataType : cutTypes) {
        for(std::shared_ptr<Channel> channel : channels) {
            //Can't start this at 0 because minimum bin will be 0, not initializing gives errors, so we set it arbitrarily high (the integer limit).
            int minimumBin = 2147483647;
            for(std::shared_ptr<Process> process : channel->getProcesses()) {
                for(auto singleProcess : process->getProcesses()) {
                    if(singleProcess.getHist(dataType, false)->GetNbinsX() < minimumBin) {
                        minimumBin = singleProcess.getHist(dataType, false)->GetNbinsX();
                    }
                }
            }
            double signal;
            double background;
            double baseSignificance;
            double significance = 0;
            int finalCut = 0;
            std::vector<TH1*> backgroundHists; 
            std::vector<TH1*> signalHists;
            std::vector<double> signalCrossSecs;
            std::vector<double> backgroundCrossSecs;
            std::vector<int> signalEvents;
            std::vector<int> backgroundEvents;
            for(std::shared_ptr<Process> process : channel->getProcesses()) {
                bool isSignal = false;
                if(process->getName() == signalName) {
                    isSignal = true;
                    for(auto singleProcess : process->getProcesses()) {
                        signalHists.push_back(singleProcess.getHist(dataType, false)->Rebin((int) ((double) singleProcess.getHist(dataType, false)->GetNbinsX() / ((double) minimumBin) + 0.5)));
                        signalEvents.push_back(singleProcess.getTotalEvents());
                        signalCrossSecs.push_back(singleProcess.getCrossSection());
                    }
                }
                if(!isSignal) {
                    for(auto singleProcess : process->getProcesses()) {
                        backgroundHists.push_back(singleProcess.getHist(dataType, false)->Rebin((int) ((double) singleProcess.getHist(dataType, false)->GetNbinsX() / ((double) minimumBin) + 0.5)));
                        backgroundEvents.push_back(singleProcess.getTotalEvents());
                        backgroundCrossSecs.push_back(singleProcess.getCrossSection());
                    }
                }
            }
            baseSignificance = calculateEvents(signalCrossSecs, signalHists, signalEvents, luminosity, 0) / (2.5 + sqrt(calculateEvents(backgroundCrossSecs, backgroundHists, backgroundEvents, luminosity, 0)));
            //std::cout << "Base significance " << baseSignificance << std::endl;
            double totalSignal = 0;
            for(int total : signalEvents) {
                totalSignal += (double) total;
            }
            double finalCutEfficiency = 1;
            for(int i = 0; i < signalHists.at(0)->GetNbinsX(); i++) {
                signal = calculateEvents(signalCrossSecs, signalHists, signalEvents, luminosity, i);
                background = calculateEvents(backgroundCrossSecs, backgroundHists, backgroundEvents, luminosity, i);
                //std::cout << "significance " << signal / (2.5 + sqrt(background)) << std::endl;
                if(signal / (2.5 + sqrt(background)) > significance) {
                    significance = signal / (2.5 + sqrt(background));
                    finalCut = i * signalHists.at(0)->GetXaxis()->GetBinWidth(0);
                    finalCutEfficiency = signal/totalSignal;
                }
            }
            std::cout << "The cut on " + histVariableNames.find(dataType)->second + " for " + channel->getName() + " is at " + finalCut + " with efficiency of " + finalCutEfficiency + " and significance difference of " + ((significance - baseSignificance) / baseSignificance) * 100 + "%" << std::endl;
        }
    }
}
