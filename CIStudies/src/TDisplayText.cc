#include "CIAnalysis/CIStudies/interface/TDisplayText.h"
#include <iostream>
#include "TObjString.h"
#include "TDialogCanvas.h"
#include "TBrowser.h"
#include "TPaveText.h"


/*
Notes:

in module.cc,
 void writeText()
*/


void TDisplayText::Browse(TBrowser* tb)
{   
    /*
    const char *name = "TDC";
    const char *title = "Description";
    Int_t wtopx = 100;
    Int_t wtopy = 100;
    unsigned ww = 500;
    unsigned wh = 300;
    auto TDC = new TDialogCanvas(name, title, wtopx, wtopy, ww, wh);

    const char *action = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    TDC->Apply(action);
    TDC->Draw();
    */
    
    char const* value = GetString().Data();

    TPaveText *newString = new TPaveText(0, 0, 1, 1);
    std::string x = value;

    newString->AddText(x.c_str());
    newString->SetTextAlign(13);
    newString->SetTextColor(kBlack);
    newString->SetTextFont(82);
    newString->Draw();
    
    
    
    
}