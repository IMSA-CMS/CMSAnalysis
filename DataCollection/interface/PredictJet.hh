#ifndef PREDICTJET_H
#define PREDICTJET_H

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "CIAnalysis/CIStudies/interface/DataStripModule.hh"
#include <list>
#include <iostream>
#include "TTree.h"

class PredictJet
{
public:
    bool PredictJet();
};
#endif