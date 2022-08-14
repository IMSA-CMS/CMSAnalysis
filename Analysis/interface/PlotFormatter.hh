#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"
#include "TString.h"

class PlotFormatter
{
    public:
        static TCanvas* formatPlot(THStack* first, THStack* second, bool drawLogo, TString extraText);
    private:
        PlotFormatter() {}
};

#endif
