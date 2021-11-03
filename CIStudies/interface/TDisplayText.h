#ifndef TDISPLAYTEXT_H
#define TDISPLAYTEXT_H

#include "TObjString.h"



class TDisplayText : public TObjString
{
public:
    void DrawPad(Option_t* option = "");// override;
    using TObjString::TObjString;   

    void Browse(TBrowser* tb) override;
private:
        ClassDef(TDisplayText,2)
};
#endif