#include "Analysis/interface/Channel.hh"
#include "Analysis/interface/Process.hh"
#include "THStack.h"

THStack* Channel::makeStack(std::string label)
{
	THStack* superPlot = new THStack(name.c_string(), name.c_string())
	if (categoeryName == "")
	{
		for (Process* process : processes)
		{
			superPlot->Add(process->getHist());
		}
	}
	else
	{
		for (Process* process : map[label])
		{
			superPlot->Add(process->getHist());
		}
	}
	return superPlot;
}