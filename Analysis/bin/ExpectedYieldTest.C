#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>
#include <fstream>	
#include "TSystem.h"
#include <memory>

void ExpectedYieldTest() {
    auto windowEst = std::make_shared<WindowEstimator>();
}