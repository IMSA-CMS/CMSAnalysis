#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"
#include <iostream>
#include "TObjString.h"
#include "TDialogCanvas.h"
#include "TBrowser.h"
#include "TPad.h"
#include "TPaveText.h"


/*
Notes:

in module.cc,
 void writeText()
*/


void TDisplayText::Browse(TBrowser* tb)
{    
    char const* value = GetString().Data();

    TPaveText *newString = new TPaveText(0, 0, 1, 1);
    std::string x = value;

    //newString->Clear();
    gPad->Clear();
    newString->AddText(x.c_str());
    newString->SetTextAlign(13);
    newString->SetTextColor(kBlack);                               
    newString->SetTextFont(82);
    newString->Draw();    
}

ClassImp(TDisplayText)
