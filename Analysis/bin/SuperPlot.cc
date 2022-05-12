#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>

int main()
{
	std::ifstream backgroundFile;
	Process* ttbarBackground = new Process("TTBar Background", 1);
	ttbarBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/TTBarPick500.root")));
	Process* zzBackground = new Process("ZZ Background", 2);
	zzBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/ZZPick4.root")));
	Process* dyBackground = new Process("DY Background", 3);
	dyBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick4.root")));
	dyBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick10.root")));
	dyBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick50.root")));
	Process* qcdBackground = new Process("QCD Background", 4);
	qcdBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick500.root")));
	qcdBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick700.root")));
	qcdBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick1000.root")));
	qcdBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick1500.root")));
	qcdBackground->addProcess(SingleProcess(new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick2000.root")));
	std::vector<Process*> backgroundProcesses = { ttbarBackground, zzBackground, dyBackground, qcdBackground };
	Channel* leptonBackgrounds = new Channel("Lepton Jet Backgrounds", backgroundProcesses);
	THStack* hist = leptonBackgrounds->getStack();
	hist->Draw();
	return 0;
}