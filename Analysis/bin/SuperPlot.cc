#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>
#include <fstream>

int main()
{
	CrossSectionReader reader("crossSections.txt");
	std::ifstream backgroundFile;
	Process* ttbarBackground = new Process("TTBar Background", 1);
	ttbarBackground->addProcess(SingleProcess("ttbar500", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/TTBarPick500.root"), &reader));
	Process* zzBackground = new Process("ZZ Background", 2);
	zzBackground->addProcess(SingleProcess("ZZ4", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/ZZPick4.root"), &reader));
	Process* dyBackground = new Process("DY Background", 3);
	dyBackground->addProcess(SingleProcess("DY4", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick4.root"), &reader));
	dyBackground->addProcess(SingleProcess("DY10", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick10.root"), &reader));
	dyBackground->addProcess(SingleProcess("DY50", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/DYPick50.root"), &reader));
	Process* qcdBackground = new Process("QCD Background", 4);
	qcdBackground->addProcess(SingleProcess("QCD500", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick500.root"), &reader));
	qcdBackground->addProcess(SingleProcess("QCD700", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick700.root"), &reader));
	qcdBackground->addProcess(SingleProcess("QCD1000", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick1000.root"), &reader));
	qcdBackground->addProcess(SingleProcess("QCD1500", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick1500.root"), &reader));
	qcdBackground->addProcess(SingleProcess("QCD2000", new RootFileInput("/uscms/home/aytang/CIStudies/bin/fullRunLepton/QCDPick2000.root"), &reader));
	std::vector<Process*> backgroundProcesses = { ttbarBackground, zzBackground, dyBackground, qcdBackground };
	Channel* leptonBackgrounds = new Channel("Lepton Jet Backgrounds", backgroundProcesses);
	THStack* hist = leptonBackgrounds->getStack();
	hist->Draw();
	return 0;
}