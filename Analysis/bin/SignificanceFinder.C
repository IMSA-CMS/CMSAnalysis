#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCutsAnalysis.hh"
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

void SignificanceFinder() {
    auto analysis = std::make_shared<HiggsCutsAnalysis>();
    //Choices are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
    std::vector<std::string> cutTypes = {"Same Sign Inv Mass"};
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    int luminosity = 3000;
    //Change this to your signal process name
    std::string signalName = "Higgs Signal";


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
    for(std::string dataType : cutTypes) {
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
                        //signalEvents.push_back(singleProcess.getTotalEvents());
                        signalEvents.push_back(singleProcess.getHist(dataType, false)->Rebin((int) ((double) singleProcess.getHist(dataType, false)->GetNbinsX() / ((double) minimumBin) + 0.5))->Integral());
                        signalCrossSecs.push_back(singleProcess.getCrossSection());
                    }
                }
                if(!isSignal) {
                    for(auto singleProcess : process->getProcesses()) {
                        backgroundHists.push_back(singleProcess.getHist(dataType, false)->Rebin((int) ((double) singleProcess.getHist(dataType, false)->GetNbinsX() / ((double) minimumBin) + 0.5)));
                        //backgroundEvents.push_back(singleProcess.getTotalEvents());
                        backgroundEvents.push_back(singleProcess.getHist(dataType, false)->Rebin((int) ((double) singleProcess.getHist(dataType, false)->GetNbinsX() / ((double) minimumBin) + 0.5))->Integral());
                        backgroundCrossSecs.push_back(singleProcess.getCrossSection());
                    }
                }
            }
            baseSignificance = calculateEvents(signalCrossSecs, signalHists, signalEvents, luminosity, 0) / (2.5 + sqrt(calculateEvents(backgroundCrossSecs, backgroundHists, backgroundEvents, luminosity, 0)));
            //std::cout << "Base significance " << baseSignificance << std::endl;
            // double totalSignal = 0;
            // for(int total : signalEvents) {
            //     totalSignal += (double) total;
            // }
            double finalCutEfficiency = 1;
            for(int i = 0; i < signalHists.at(0)->GetNbinsX(); i++) {
                signal = calculateEvents(signalCrossSecs, signalHists, signalEvents, luminosity, i);
                background = calculateEvents(backgroundCrossSecs, backgroundHists, backgroundEvents, luminosity, i);

                //std::cout << "significance " << signal / (2.5 + sqrt(background)) << std::endl;
                significance = signal / (2.5 + sqrt(background));
                finalCut = i * signalHists.at(0)->GetXaxis()->GetBinWidth(0);
                finalCutEfficiency = signal/calculateEvents(signalCrossSecs, signalHists, signalEvents, luminosity, 0);
                //finalCutEfficiency = signal/totalSignal;
                std::cout << "The cut on " + dataType + " for " + channel->getName() + " at " + finalCut + " has efficiency of " + finalCutEfficiency + " and significance difference of " + ((significance - baseSignificance) / baseSignificance) * 100 + "%" << std::endl;
                std::cout << "This cut keeps " << signal << " signal events and " << background << " background events." << std::endl;
            }
        }
    }
}
