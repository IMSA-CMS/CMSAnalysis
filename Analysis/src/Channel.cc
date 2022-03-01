#include "Analysis/interface/Channel.hh"
#include "Analysis/interface/Process.hh"
#include "THStack.h"

THStack* Channel.makeStack()
{
	THStack* superPlot = new THStack(name.c_string(), name.c_string())
	for (Process* process : processes)
	{
		superPlot->Add(process->getHist());
	}
	return superPlot;
}