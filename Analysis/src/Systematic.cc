#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Systematic.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include "CMSAnalysis/Utility/interface/Utility.hh"

Systematic::Systematic(std::string iName):
name(iName)
{
	// finds the total changes for the over and underestimates of the graph
}

static std::shared_ptr <Systematic> makeSystematicFromHist(TH1*original,TH1*up,TH1*down)
{
	return nullptr;
}