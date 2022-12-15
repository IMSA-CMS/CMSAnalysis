#ifndef TDISPLAYTEXT_H
#define TDISPLAYTEXT_H

#include "TObjString.h"

//Makes text for root files, usually to display NEvents

class TDisplayText : public TObjString
{
    public:
        TDisplayText(TBrowser* tb);
        void DrawPad(Option_t* option = "");
        using TObjString::TObjString;

        //Adds the displayText to the browser
        void Browse(TBrowser* tb) override;
    private:
        ClassDef(TDisplayText,2)
};
#endif